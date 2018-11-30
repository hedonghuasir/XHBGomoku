//
//  js_message.cpp
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <algorithm>
#include <google/protobuf/stubs/hash.h>
#include <map>
#include <utility>
#include <vector>
#include <google/protobuf/compiler/js/js_message.h>
#include <google/protobuf/compiler/js/js_enum.h>
#include <google/protobuf/compiler/js/js_helpers.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/descriptor.pb.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace js {
    
using internal::WireFormat;
using internal::WireFormatLite;

const char* kWireTypeNames[] = {
    "VARINT",
    "FIXED64",
    "LENGTH_DELIMITED",
    "START_GROUP",
    "END_GROUP",
    "FIXED32",
};
    
MessageGenerator::MessageGenerator(const Descriptor* descriptor)
    : descriptor_(descriptor),
      nested_generators_(new scoped_ptr<MessageGenerator>[descriptor->nested_type_count()]),
      enum_generators_(new scoped_ptr<EnumGenerator>[descriptor->enum_type_count()]),
      field_generators_(descriptor)
{
    fullname_ = ObjectFullName(descriptor_);
    for (int i = 0; i < descriptor->nested_type_count(); i++)
    {
        nested_generators_[i].reset(new MessageGenerator(descriptor->nested_type(i)));
    }
    
    for (int i = 0; i < descriptor->enum_type_count(); i++)
    {
        enum_generators_[i].reset(new EnumGenerator(descriptor->enum_type(i)));
    }
}
    
void MessageGenerator::GenerateEnumDefinitions(io::Printer* printer)
{
    for (int i = 0; i < descriptor_->enum_type_count(); i++)
    {
        enum_generators_[i]->Generate(printer);
    }
}

void MessageGenerator::GenerateMessageDefinition(io::Printer* printer)
{
    printer->Print("exports.$fullname$ = function() {\n", "fullname", fullname_);
    printer->Indent();
    
    GenerateFields(printer);
    GenerateByteSize(printer);
    GenerateClear(printer);
    GenerateCopyFromJSObject(printer);
    GenerateCopyToJSObject(printer);
    GenerateMergeFromCodedStream(printer);
    GenerateSerializeToCodedStream(printer);
    
    printer->Outdent();
    printer->Print("};\n\n");
}
    
void MessageGenerator::GenerateNestedMessage(io::Printer* printer)
{
    for (int i = 0; i < descriptor_->nested_type_count(); i++) {
        nested_generators_[i]->GenerateMessageDefinition(printer);
        nested_generators_[i]->GenerateEnumDefinitions(printer);
        nested_generators_[i]->GenerateNestedMessage(printer);
    }
}
    
MessageGenerator::~MessageGenerator() {}
    
void MessageGenerator::GenerateFields(io::Printer* printer)
{
    // Generate field declarations and accessors
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field)
            .GenerateMembers(printer);
        printer->Print("\n");
    }
    printer->Print("\n");
}

void MessageGenerator::GenerateByteSize(io::Printer* printer)
{
    printer->Print("this.getByteSize = function() {\n");
    printer->Indent();
    printer->Print("var total_size = 0;\n");
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field).GenerateByteSizeCode(printer);
        printer->Print("\n");
    }
    printer->Print("return total_size;\n");
    printer->Outdent();
    printer->Print("};\n\n");
}
    
void MessageGenerator::GenerateClear(io::Printer* printer)
{
    printer->Print("this.clear = function() {\n");
    printer->Indent();
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field).GenerateClearCode(printer);
        printer->Print("\n");
    }
    printer->Outdent();
    printer->Print("};\n\n");
}

void MessageGenerator::GenerateCopyFromJSObject(io::Printer* printer)
{
    printer->Print("this.copyFromJSObject = function(obj) {\n");
    printer->Indent();
    printer->Print("this.clear();\n");
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field).GenerateCopyFromJSObjectCode(printer);
    }
    printer->Outdent();
    printer->Print("};\n\n");
}

void MessageGenerator::GenerateCopyToJSObject(io::Printer* printer)
{
    printer->Print("this.copyToJSObject = function(obj) {\n");
    printer->Indent();
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field).GenerateCopyToJSObjectCode(printer);
    }
    printer->Outdent();
    printer->Print("};\n\n");
}

void MessageGenerator::GenerateMergeFromCodedStream(io::Printer* printer)
{
    printer->Print("this.mergePartialFromCodedStream = function(input) {\n");
    printer->Indent();
    printer->Print("var done = false;\n"
                   "while (!done) {\n"
                   "  var tag = input.readTag();\n");
    printer->Indent();
    printer->Print("switch (tag) {\n");
    printer->Indent();
    printer->Print("case 0:\n"
                   "  done = true;\n"
                   "  break;\n");
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        uint32 tag = WireFormatLite::MakeTag(field->number(),
                                             WireFormat::WireTypeForFieldType(field->type()));
        printer->Print(
                       "case $tag$: {\n",
                       "tag", SimpleItoa(tag));
        printer->Indent();
        field_generators_.get(field).GenerateParsingCode(printer);
        
        printer->Outdent();
        printer->Print(
                       "  break;\n"
                       "}\n");
        
        if (field->is_packable()) {
            // To make packed = true wire compatible, we generate parsing code from a
            // packed version of this field regardless of field->options().packed().
            uint32 packed_tag = WireFormatLite::MakeTag(field->number(),
                                                        WireFormatLite::WIRETYPE_LENGTH_DELIMITED);
            printer->Print(
                           "case $tag$: {\n",
                           "tag", SimpleItoa(packed_tag));
            printer->Indent();
            
            field_generators_.get(field).GenerateParsingCodeFromPacked(printer);
            
            printer->Outdent();
            printer->Print(
                           "  break;\n"
                           "}\n");
        }
    }
    printer->Print("default : {\n"
                   "  if (!input.skipField(tag))\n"
                   "    done = true;\n"
                   "  break;\n");
    printer->Print("}\n");
    printer->Outdent();
    printer->Print("}\n");
    printer->Outdent();
    printer->Print("}\n");
    printer->Print("return true;\n");
    printer->Outdent();
    printer->Print("};\n\n");
    
    printer->Print("this.parseFromArrayBuffer = function(input) {\n");
    printer->Indent();
    printer->Print("this.clear();\n");
    printer->Print("var istream = new CodedInputStream(input);\n");
    printer->Print("return this.mergePartialFromCodedStream(istream);\n");
    printer->Outdent();
    printer->Print("};\n\n");
}

void MessageGenerator::GenerateSerializeToCodedStream(io::Printer* printer)
{
    printer->Print("this.serializePartialToCodedStream = function(output) {\n");
    printer->Indent();
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field).GenerateSerializationCode(printer);
    }
    printer->Outdent();
    printer->Print("};\n\n");
    
    printer->Print("this.serializeToArrayBuffer = function() {\n");
    printer->Indent();
    printer->Print("var size = this.getByteSize();\n");
    printer->Print("var output = new CodedOutputStream(size);\n");
    printer->Print("this.serializePartialToCodedStream(output);\n");
    printer->Print("return output.getBuffer();\n");
    printer->Outdent();
    printer->Print("};\n\n");
}
    
void MessageGenerator::GenerateEnumDefinitionsTSD(io::Printer* printer)
{
    for (int i = 0; i < descriptor_->enum_type_count(); i++)
    {
        enum_generators_[i]->GenerateTSD(printer);
    }
}
    
void MessageGenerator::GenerateClassDeclareTSD(io::Printer* printer)
{
    if (fullname_ != descriptor_->name()) {
        printer->Print("static $name$ = class implements Message {\n", "name", descriptor_->name());
    } else {
        printer->Print("class $fullname$ implements Message {\n", "fullname", fullname_);
    }
    printer->Indent();
    
    printer->Print("parseFromArrayBuffer(buffer: ArrayBuffer): void;\n");
    printer->Print("serializeToArrayBuffer(): ArrayBuffer;\n");
    printer->Print("copyFromJSObject(obj: Object): void;\n");
    printer->Print("copyToJSObject(obj: Object): void;\n\n");
    GenerateFieldsTSD(printer);
    
    for (int i = 0; i < descriptor_->nested_type_count(); i++) {
        nested_generators_[i]->GenerateClassDeclareTSD(printer);
    }
    printer->Outdent();
    printer->Print("}\n\n");
}

void MessageGenerator::GenerateMessageDefinitionTSD(io::Printer* printer)
{
    this->GenerateEnumDefinitions(printer);
    this->GenerateInterfaceMessageTSD(printer);
    this->GenerateClassDeclareTSD(printer);
}
    
void MessageGenerator::GenerateFieldsTSD(io::Printer* printer)
{
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field)
        .GenerateFieldMethodDeclareTSD(printer);
    }
}

void MessageGenerator::GenerateInterfaceMessageTSD(io::Printer* printer)
{
    printer->Print("interface $fullname$_I {\n", "fullname", TSInterfaceFullName(descriptor_));
    printer->Indent();
    
    for (int i = 0; i < descriptor_->field_count(); i++) {
        const FieldDescriptor* field = descriptor_->field(i);
        field_generators_.get(field)
        .GenerateFieldDeclareTSD(printer);
    }
    printer->Outdent();
    printer->Print("}\n");
    
    for (int i = 0; i < descriptor_->nested_type_count(); i++) {
        nested_generators_[i]->GenerateEnumDefinitionsTSD(printer);
        nested_generators_[i]->GenerateInterfaceMessageTSD(printer);
    }
}
    
}
}
}
}
