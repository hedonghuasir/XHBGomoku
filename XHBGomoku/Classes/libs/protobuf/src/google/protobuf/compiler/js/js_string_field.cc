//
//  js_string_field.cpp
//  protobuf
//
//  Created by zhao shuan on 8/3/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <map>
#include <string>

#include <google/protobuf/compiler/js/js_string_field.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/js/js_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/stubs/strutil.h>
namespace google {
namespace protobuf {
namespace compiler {
namespace js {

using internal::WireFormat;
using internal::WireFormatLite;

namespace {
    
void SetPrimitiveVariables(const FieldDescriptor* descriptor,
                           map<string, string>* variables) {
    (*variables)["name"] = FieldName(descriptor);
    (*variables)["number"] = SimpleItoa(descriptor->number());
    (*variables)["default"] = DefaultValue(descriptor);
    (*variables)["default_by_type"] = DefaultValueByType(descriptor);
    (*variables)["capitalized_type"] = "String";
    (*variables)["tag"] = SimpleItoa(WireFormat::MakeTag(descriptor));
    (*variables)["tag_size"] = SimpleItoa(WireFormat::TagSize(descriptor->number(), descriptor->type()));
}
    
}  // namespace

// ===================================================================

StringFieldGenerator::
StringFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor)
{
    SetPrimitiveVariables(descriptor, &variables_);
}

StringFieldGenerator::~StringFieldGenerator() {}
    
void StringFieldGenerator::GenerateMembers(io::Printer* printer) const
{
    if (descriptor_->has_default_value())
    {
        printer->Print(variables_, "var _$name$ = $default$;\n");
    }
    else
    {
        printer->Print(variables_, "var _$name$ = $default_by_type$;\n");
    }
    printer->Print(variables_,
                   "var _has_$name$ = false;\n"
                   "this.$name$ = function() {return _$name$;};\n"
                   "this.has_$name$ = function() {return _has_$name$;};\n");
}
    
void StringFieldGenerator::GenerateFieldMethodDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$(): string;\n");
    printer->Print(variables_, "has_$name$(): boolean;\n");
}
    
void StringFieldGenerator::GenerateFieldDeclareTSD(io::Printer* printer) const
{
    if (descriptor_->is_optional())
    {
        printer->Print(variables_, "$name$?: string;\n");
    }
    else
    {
        printer->Print(variables_, "$name$: string;\n");
    }
}
    
void StringFieldGenerator::GenerateClearCode(io::Printer* printer) const
{
    if (descriptor_->has_default_value())
    {
        printer->Print(variables_, "_$name$ = $default$;\n");
    }
    else
    {
        printer->Print(variables_, "_$name$ = $default_by_type$;\n");
    }
    printer->Print(variables_, "_has_$name$ = false;\n");
}
    
void StringFieldGenerator::GenerateCopyFromJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_, "if (obj.$name$ != undefined) {\n"
                   "  _$name$ = obj.$name$;\n"
                   "  _has_$name$ = true;\n"
                   "}\n");
}

void StringFieldGenerator::GenerateCopyToJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_, "obj.$name$ = _$name$;\n");
}

void StringFieldGenerator::GenerateParsingCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "_$name$ = input.readString();\n"
                   "_has_$name$ = true;\n");
}

void StringFieldGenerator::GenerateSerializationCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (_has_$name$) {\n"
                   "  output.writeString($number$, _$name$);\n"
                   "}\n");
}

void StringFieldGenerator::GenerateByteSizeCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (_has_$name$) {\n"
                   "  total_size += CodedOutputStream.computeStringSize($number$, _$name$);\n"
                   "}\n");
    
}
    
RepeatedStringFieldGenerator::RepeatedStringFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor)
{
    SetPrimitiveVariables(descriptor, &variables_);
}

RepeatedStringFieldGenerator::~RepeatedStringFieldGenerator()
{
    
}

void RepeatedStringFieldGenerator::GenerateMembers(io::Printer* printer) const
{
    if (descriptor_->options().packed()) {
        printer->Print(variables_,
                       "var _$name$MemoizedSerializedSize = 0;\n");
    }
    printer->Print(variables_,
                   "var _$name$ = [];\n"
                   "this.$name$ = function(i) {\n"
                   "  if (i == undefined) {\n"
                   "    return _$name$.slice(0);\n"
                   "  } else {\n"
                   "    return _$name$[i];\n"
                   "  }"
                   "};\n"
                   "this.$name$_size = function() {return _$name$.length;};\n");
}
    
void RepeatedStringFieldGenerator::GenerateFieldMethodDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$(i?: number): string|string[];\n");
    printer->Print(variables_, "$name$_size(): number;\n");
}
    
void RepeatedStringFieldGenerator::GenerateFieldDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$?: string[];\n");
}
    
void RepeatedStringFieldGenerator::GenerateClearCode(io::Printer* printer) const
{
    printer->Print(variables_, "_$name$ = [];\n");
}
    
void RepeatedStringFieldGenerator::GenerateCopyFromJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (obj.$name$ && obj.$name$ instanceof Array) {\n"
                   "  _$name$ = obj.$name$.slice(0);\n"
                   "}\n");
}

void RepeatedStringFieldGenerator::GenerateCopyToJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_, "obj.$name$ = _$name$.slice(0);\n");
}

void RepeatedStringFieldGenerator::GenerateParsingCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (!_$name$) {\n"
                   "  _$name$ = [];\n"
                   "}\n"
                   "_$name$.push(input.readString());\n");
}

void RepeatedStringFieldGenerator::GenerateParsingCodeFromPacked(io::Printer* printer) const
{
    printer->Print(variables_,
                   "var length = input.readRawVarint32();\n"
                   "var limit = input.pushLimit(length);\n"
                   "if (!_$name$ && input.getBytesUntilLimit() > 0) {\n"
                   "  _$name$ = [];\n"
                   "}\n"
                   "while (input.getBytesUntilLimit() > 0) {\n"
                   "  _$name$.push(input.readString());\n"
                   "}\n"
                   "input.popLimit(limit);\n");
}

void RepeatedStringFieldGenerator::GenerateSerializationCode(io::Printer* printer) const
{
    if (descriptor_->options().packed()) {
        printer->Print(variables_,
                       "if (_$name$.length > 0) {\n"
                       "  output.writeRawVarint32($tag$);\n"
                       "  output.writeRawVarint32(_$name$MemoizedSerializedSize);\n"
                       "}\n"
                       "for (var i = 0; i < _$name$.length; ++i) {\n"
                       "  output.write$capitalized_type$NoTag(_$name$[i]);\n"
                       "}\n");
    } else {
        printer->Print(variables_,
                       "for (var i = 0; i < _$name$.length; ++i) {\n"
                       "  output.writeString($number$, _$name$[i]);\n"
                       "}\n");
    }
}

void RepeatedStringFieldGenerator::GenerateByteSizeCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "{\n"
                   "  var dataSize = 0;\n");
    printer->Indent();
    
    printer->Print(variables_,
                   "for (var i = 0; i < _$name$.length; ++i) {\n"
                   "  dataSize += CodedOutputStream\n"
                   "    .computeStringSizeNoTag(_$name$[i]);\n"
                   "}\n");
    
    printer->Print(
                   "total_size += dataSize;\n");
    
    if (descriptor_->options().packed()) {
        printer->Print(variables_,
                       "if (_$name$.length) {\n"
                       "  total_size += $tag_size$;\n"
                       "  total_size += CodedOutputStream\n"
                       "      .computeInt32SizeNoTag(dataSize);\n"
                       "}\n");
    } else {
        printer->Print(variables_,
                       "total_size += $tag_size$ * _$name$.length;\n");
    }
    
    // cache the data size for packed fields.
    if (descriptor_->options().packed()) {
        printer->Print(variables_,
                       "_$name$MemoizedSerializedSize = dataSize;\n");
    }
    
    printer->Outdent();
    printer->Print("}\n");
}

}
}
}
}
