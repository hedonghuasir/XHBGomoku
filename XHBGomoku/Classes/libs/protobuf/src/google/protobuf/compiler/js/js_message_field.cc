//
//  js_message_field.cpp
//  protobuf
//
//  Created by zhao shuan on 8/3/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <map>
#include <string>

#include <google/protobuf/compiler/js/js_message_field.h>
#include <google/protobuf/compiler/js/js_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {

namespace {

// TODO(kenton):  Factor out a "SetCommonFieldVariables()" to get rid of
//   repeat code between this and the other field types.
void SetMessageVariables(const FieldDescriptor* descriptor,
                         map<string, string>* variables) {
    (*variables)["name"] = FieldName(descriptor);
    (*variables)["number"] = SimpleItoa(descriptor->number());
    (*variables)["tag"] = SimpleItoa(internal::WireFormat::MakeTag(descriptor));
    (*variables)["tag_size"] = SimpleItoa(internal::WireFormat::TagSize(descriptor->number(), descriptor->type()));
    (*variables)["type"] =
        DependencyVarName(descriptor->message_type()->file()) + "." + ObjectFullName(descriptor->message_type());
    (*variables)["tsd_interface_type"] = DependencyVarName(descriptor->message_type()->file()) + "." + TSInterfaceFullName(descriptor->message_type());
    (*variables)["group_or_message"] =
        (descriptor->type() == FieldDescriptor::TYPE_GROUP) ?
        "Group" : "Message";
    
}

}  // namespace

// ===================================================================

MessageFieldGenerator::
MessageFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor)
{
    SetMessageVariables(descriptor, &variables_);
}

MessageFieldGenerator::~MessageFieldGenerator() {}
    
void MessageFieldGenerator::GenerateMembers(io::Printer* printer) const
{
    printer->Print(variables_,
                   "var _$name$ = null;\n"
                   "var _has_$name$ = false;\n"
                   "this.$name$ = function() {return _$name$;};\n"
                   "this.has_$name$ = function() {return _has_$name$;};\n");
}
    
void MessageFieldGenerator::GenerateFieldMethodDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$(): $type$;\n");
    printer->Print(variables_, "has_$name$(): boolean;\n");
}

void MessageFieldGenerator::GenerateFieldDeclareTSD(io::Printer* printer) const
{
    if (descriptor_->is_optional())
    {
        printer->Print(variables_, "$name$?: $tsd_interface_type$_I;\n");
    }
    else
    {
        printer->Print(variables_, "$name$: $tsd_interface_type$_I;\n");
    }
}
    
void MessageFieldGenerator::GenerateClearCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "_$name$ = null;\n"
                   "_has_$name$ = false;\n");
}
    
void MessageFieldGenerator::GenerateCopyFromJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (obj.$name$ != undefined) {\n"
                   "  _$name$ = new $type$();\n"
                   "  _$name$.copyFromJSObject(obj.$name$);\n"
                   "  _has_$name$ = true;\n"
                   "}\n");
}

void MessageFieldGenerator::GenerateCopyToJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (_$name$) {\n"
                   "  obj.$name$ = {};\n"
                   "  _$name$.copyToJSObject(obj.$name$);\n"
                   "}\n");
}

void MessageFieldGenerator::GenerateParsingCode(io::Printer* printer) const
{
    printer->Print(variables_, "_$name$ = new $type$();\n");
    if (descriptor_->type() == FieldDescriptor::TYPE_GROUP) {
        printer->Print(variables_,
                       "input.readGroup($number$, _$name$);\n");
    } else {
        printer->Print(variables_,
                       "input.readMessage(_$name$);\n");
    }
    printer->Print(variables_,
                   "_has_$name$ = true;\n");
}

void MessageFieldGenerator::GenerateSerializationCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (_has_$name$) {\n"
                   "  output.write$group_or_message$($number$, _$name$);\n"
                   "}\n");
}

void MessageFieldGenerator::GenerateByteSizeCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (_has_$name$) {\n"
                   "  total_size += CodedOutputStream.computeMessageSize($number$, _$name$);\n"
                   "}\n");
}
    
RepeatedMessageFieldGenerator::RepeatedMessageFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor)
{
    SetMessageVariables(descriptor, &variables_);
}

RepeatedMessageFieldGenerator::~RepeatedMessageFieldGenerator()
{
    
}

void RepeatedMessageFieldGenerator::GenerateMembers(io::Printer* printer) const
{
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
    
void RepeatedMessageFieldGenerator::GenerateFieldMethodDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$(i?: number): $type$|$type$[];\n");
    printer->Print(variables_, "$name$_size(): number;\n");
}

void RepeatedMessageFieldGenerator::GenerateFieldDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$?: $tsd_interface_type$_I[];\n");
}
    
void RepeatedMessageFieldGenerator::GenerateClearCode(io::Printer* printer) const
{
    printer->Print(variables_, "_$name$ = [];\n");
}
    
void RepeatedMessageFieldGenerator::GenerateCopyFromJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (obj.$name$ && obj.$name$ instanceof Array) {\n"
                   "  _$name$ = [];\n");
    printer->Indent();
    printer->Print(variables_,
                   "for (var i = 0; i < obj.$name$.length; ++i) {\n"
                   "  var tmp = new $type$();\n"
                   "  tmp.copyFromJSObject(obj.$name$[i]);\n"
                   "  _$name$.push(tmp);\n"
                   "}\n");
    printer->Outdent();
    printer->Print("}\n");
}

void RepeatedMessageFieldGenerator::GenerateCopyToJSObjectCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "obj.$name$ = [];\n"
                   "for (var i = 0; i < _$name$.length; ++i) {\n"
                   "  var tmp = {};\n"
                   "  _$name$[i].copyToJSObject(tmp);\n"
                   "  obj.$name$.push(tmp);\n"
                   "}\n");
}

void RepeatedMessageFieldGenerator::GenerateParsingCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (!_$name$) {\n"
                   "  _$name$ = [];\n"
                   "}\n");
    printer->Print(variables_, "var tmp = new $type$();\n");
    if (descriptor_->type() == FieldDescriptor::TYPE_GROUP) {
        printer->Print(variables_,
                       "input.readGroup($number$, tmp);\n");
    } else {
        printer->Print(variables_,
                       "input.readMessage(tmp);\n");
    }
    printer->Print(variables_, "_$name$.push(tmp);\n");
}

void RepeatedMessageFieldGenerator::GenerateSerializationCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "for (var i = 0; i < _$name$.length; ++i) {\n"
                   "  output.write$group_or_message$($number$, _$name$[i]);\n"
                   "}\n");
}

void RepeatedMessageFieldGenerator::GenerateByteSizeCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "for (var i = 0; i < _$name$.length; ++i) {\n"
                   "  total_size += CodedOutputStream\n"
                   "    .compute$group_or_message$Size($number$, _$name$[i]);\n"
                   "}\n");
}
    
}
}
}
}
