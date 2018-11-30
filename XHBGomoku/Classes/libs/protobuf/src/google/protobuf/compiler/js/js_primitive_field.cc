//
//  js_primitive_field.cpp
//  protobuf
//
//  Created by zhao shuan on 8/3/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <map>
#include <string>

#include <google/protobuf/compiler/js/js_primitive_field.h>
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
// For encodings with fixed sizes, returns that size in bytes.  Otherwise
// returns -1.
int FixedSize(FieldDescriptor::Type type) {
    switch (type) {
        case FieldDescriptor::TYPE_INT32   : return -1;
        case FieldDescriptor::TYPE_INT64   : return -1;
        case FieldDescriptor::TYPE_UINT32  : return -1;
        case FieldDescriptor::TYPE_UINT64  : return -1;
        case FieldDescriptor::TYPE_SINT32  : return -1;
        case FieldDescriptor::TYPE_SINT64  : return -1;
        case FieldDescriptor::TYPE_FIXED32 : return WireFormatLite::kFixed32Size;
        case FieldDescriptor::TYPE_FIXED64 : return WireFormatLite::kFixed64Size;
        case FieldDescriptor::TYPE_SFIXED32: return WireFormatLite::kSFixed32Size;
        case FieldDescriptor::TYPE_SFIXED64: return WireFormatLite::kSFixed64Size;
        case FieldDescriptor::TYPE_FLOAT   : return WireFormatLite::kFloatSize;
        case FieldDescriptor::TYPE_DOUBLE  : return WireFormatLite::kDoubleSize;
            
        case FieldDescriptor::TYPE_BOOL    : return WireFormatLite::kBoolSize;
        case FieldDescriptor::TYPE_ENUM    : return -1;
            
        case FieldDescriptor::TYPE_STRING  : return -1;
        case FieldDescriptor::TYPE_BYTES   : return -1;
        case FieldDescriptor::TYPE_GROUP   : return -1;
        case FieldDescriptor::TYPE_MESSAGE : return -1;
            
            // No default because we want the compiler to complain if any new
            // types are added.
    }
    GOOGLE_LOG(FATAL) << "Can't get here.";
    return -1;
}

void SetPrimitiveVariables(const FieldDescriptor* descriptor,
                           map<string, string>* variables) {
    (*variables)["name"] = FieldName(descriptor);
    (*variables)["number"] = SimpleItoa(descriptor->number());
    (*variables)["default"] = DefaultValue(descriptor);
    (*variables)["default_by_type"] = DefaultValueByType(descriptor);
    (*variables)["capitalized_type"] = GetCapitalizedType(descriptor);
    (*variables)["tstype"] = GetTSTypeString(descriptor);
    (*variables)["tag"] = SimpleItoa(WireFormat::MakeTag(descriptor));
    (*variables)["tag_size"] = SimpleItoa(WireFormat::TagSize(descriptor->number(), descriptor->type()));
    
    int fixed_size = FixedSize(descriptor->type());
    if (fixed_size != -1) {
        (*variables)["fixed_size"] = SimpleItoa(fixed_size);
    }
}

}
    
PrimitiveFieldGenerator::
PrimitiveFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor)
{
    SetPrimitiveVariables(descriptor, &variables_);
}

PrimitiveFieldGenerator::~PrimitiveFieldGenerator() {}
    
void PrimitiveFieldGenerator::GenerateMembers(io::Printer* printer) const
{
    if (descriptor_->has_default_value()) {
        printer->Print(variables_, "var _$name$ = $default$;\n");
    }
    else
    {
        printer->Print(variables_, "var _$name$ = $default_by_type$;\n");
    }
    printer->Print(variables_, "var _has_$name$ = false;\n");
    if (GetJSType(descriptor_)  == JSTYPE_INT64)
    {
        printer->Print(variables_, "this.$name$ = function() {return _$name$.toNumber();};\n");
    }
    else
    {
        printer->Print(variables_, "this.$name$ = function() {return _$name$;};\n");
    }
    printer->Print(variables_, "this.has_$name$ = function() {return _has_$name$;};\n");
}
    
void PrimitiveFieldGenerator::GenerateFieldMethodDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$(): $tstype$;\n");
    printer->Print(variables_, "has_$name$(): boolean;\n");
}
    
void PrimitiveFieldGenerator::GenerateFieldDeclareTSD(io::Printer* printer) const
{
    if (descriptor_->is_optional())
    {
        printer->Print(variables_, "$name$?: $tstype$;\n");
    }
    else
    {
        printer->Print(variables_, "$name$: $tstype$;\n");
    }
}
    
void PrimitiveFieldGenerator::GenerateClearCode(io::Printer* printer) const
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

void PrimitiveFieldGenerator::GenerateCopyFromJSObjectCode(io::Printer* printer) const
{
    if (GetJSType(descriptor_)  == JSTYPE_INT64)
    {
        printer->Print(variables_, "if (obj.$name$ != undefined) {\n");
        printer->Indent();
        if (descriptor_->cpp_type() == FieldDescriptor::CPPTYPE_INT64)
            printer->Print(variables_, "_$name$ = Int64.fromValue(obj.$name$).toSigned();\n");
        else
            printer->Print(variables_, "_$name$ = Int64.fromValue(obj.$name$).toUnsigned();\n");
        printer->Print(variables_, "_has_$name$ = true;\n");
        printer->Outdent();
        printer->Print("}\n");
    }
    else
    {
        printer->Print(variables_,
                       "if (obj.$name$ != undefined) {\n"
                       "  _$name$ = obj.$name$;\n"
                       "  _has_$name$ = true;\n"
                       "}\n");
    }
}

void PrimitiveFieldGenerator::GenerateCopyToJSObjectCode(io::Printer* printer) const
{
    if (GetJSType(descriptor_)  == JSTYPE_INT64)
    {
        printer->Print(variables_,
                       "if (_$name$ != undefined) {\n"
                       "  obj.$name$ = _$name$.toNumber();\n"
                       "}\n");
    }
    else
    {
        printer->Print(variables_, "obj.$name$ = _$name$;\n");
    }
}

void PrimitiveFieldGenerator::GenerateParsingCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "_$name$ = input.read$capitalized_type$();\n"
                   "_has_$name$ = true;\n");
}

void PrimitiveFieldGenerator::GenerateSerializationCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (_has_$name$) {\n"
                   "  output.write$capitalized_type$($number$, _$name$);\n"
                   "}\n");
}

void PrimitiveFieldGenerator::GenerateByteSizeCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (_has_$name$) {\n"
                   "  total_size += CodedOutputStream.compute$capitalized_type$Size($number$, _$name$);\n"
                   "}\n");
}
    
RepeatedPrimitiveFieldGenerator::RepeatedPrimitiveFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor)
{
    SetPrimitiveVariables(descriptor, &variables_);
}

RepeatedPrimitiveFieldGenerator::~RepeatedPrimitiveFieldGenerator() {}

void RepeatedPrimitiveFieldGenerator::GenerateMembers(io::Printer* printer) const
{
    if (descriptor_->options().packed()) {
        printer->Print(variables_,
                       "var _$name$MemoizedSerializedSize = 0;\n");
    }
    printer->Print(variables_, "var _$name$ = [];\n");
    if (GetJSType(descriptor_)  == JSTYPE_INT64)
    {
        printer->Print(variables_,
                       "this.$name$ = function(i) {\n"
                       "  if (i == undefined) {\n"
                       "    var res = [];\n"
                       "    for (var i = 0; i < _$name$.length; ++i) {\n"
                       "      res.push(_$name$[i].toNumber());\n"
                       "    }\n"
                       "    return res;\n"
                       "  } else {\n"
                       "    return _$name$[i].toNumber();\n"
                       "  }\n"
                       "};\n");
    }
    else
    {
        printer->Print(variables_,
                       "this.$name$ = function(i) {\n"
                       "  if (i == undefined) {\n"
                       "    return _$name$.slice(0);\n"
                       "  } else {\n"
                       "    return _$name$[i];\n"
                       "  }"
                       "};\n");
    }
    printer->Print(variables_, "this.$name$_size = function() {return _$name$.length;};\n");
}
    
void RepeatedPrimitiveFieldGenerator::GenerateFieldMethodDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$(i?: number): $tstype$|$tstype$[];\n");
    printer->Print(variables_, "$name$_size(): number;\n");
}
    
void RepeatedPrimitiveFieldGenerator::GenerateFieldDeclareTSD(io::Printer* printer) const
{
    printer->Print(variables_, "$name$?: $tstype$[];\n");
}
    
void RepeatedPrimitiveFieldGenerator::GenerateClearCode(io::Printer* printer) const
{
    printer->Print(variables_, "_$name$ = [];\n");
}

void RepeatedPrimitiveFieldGenerator::GenerateCopyFromJSObjectCode(io::Printer* printer) const
{
    if (GetJSType(descriptor_)  == JSTYPE_INT64)
    {
        printer->Print(variables_,
                       "if (obj.$name$ && obj.$name$ instanceof Array) {\n"
                       "  _$name$ = [];\n");
        printer->Indent();
        printer->Print(variables_, "for (var i = 0; i < obj.$name$.length; ++i) {\n");
        printer->Indent();
        if (descriptor_->cpp_type() == FieldDescriptor::CPPTYPE_INT64)
            printer->Print(variables_, "_$name$.push(Int64.fromValue(obj.$name$[i]).toSigned());\n");
        else
            printer->Print(variables_, "_$name$.push(Int64.fromValue(obj.$name$[i]).toUnsigned());\n");
        printer->Outdent();
        printer->Print("}\n");
        printer->Outdent();
        printer->Print("}\n");
    }
    else
    {
        printer->Print(variables_,
                       "if (obj.$name$ && obj.$name$ instanceof Array) {\n"
                       "  _$name$ = obj.$name$.slice(0);\n"
                       "}\n");
    }
}

void RepeatedPrimitiveFieldGenerator::GenerateCopyToJSObjectCode(io::Printer* printer) const
{
    if (GetJSType(descriptor_)  == JSTYPE_INT64)
    {
        printer->Print(variables_,
                       "obj.$name$ = [];\n"
                       "for (var i = 0; i < _$name$.length; ++i) {\n"
                       "  obj.$name$.push(_$name$[i].toNumber());\n"
                       "}\n");
    }
    else
    {
        printer->Print(variables_, "obj.$name$ = _$name$.slice(0);\n");
    }
}

void RepeatedPrimitiveFieldGenerator::GenerateParsingCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "if (!_$name$) {\n"
                   "  _$name$ = [];\n"
                   "}\n"
                   "_$name$.push(input.read$capitalized_type$());\n");
}

void RepeatedPrimitiveFieldGenerator::GenerateParsingCodeFromPacked(io::Printer* printer) const
{
    printer->Print(variables_,
                   "var length = input.readRawVarint32();\n"
                   "var limit = input.pushLimit(length);\n"
                   "if (!_$name$ && input.getBytesUntilLimit() > 0) {\n"
                   "  _$name$ = [];\n"
                   "}\n"
                   "while (input.getBytesUntilLimit() > 0) {\n"
                   "  _$name$.push(input.read$capitalized_type$());\n"
                   "}\n"
                   "input.popLimit(limit);\n");
}

void RepeatedPrimitiveFieldGenerator::GenerateSerializationCode(io::Printer* printer) const
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
                       "  output.write$capitalized_type$($number$, _$name$[i]);\n"
                       "}\n");
    }
}

void RepeatedPrimitiveFieldGenerator::GenerateByteSizeCode(io::Printer* printer) const
{
    printer->Print(variables_,
                   "{\n"
                   "  var dataSize = 0;\n");
    printer->Indent();
    
    if (FixedSize(descriptor_->type()) == -1) {
        printer->Print(variables_,
                       "for (var i = 0; i < _$name$.length; i++) {\n"
                       "  dataSize += CodedOutputStream\n"
                       "    .compute$capitalized_type$SizeNoTag(_$name$[i]);\n"
                       "}\n");
    } else {
        printer->Print(variables_,
                       "dataSize = $fixed_size$ * _$name$.length;\n");
    }
    
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
