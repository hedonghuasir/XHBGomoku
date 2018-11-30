//
//  js_helpers.cpp
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <google/protobuf/compiler/js/js_helpers.h>

#include <limits>
#include <map>
#include <vector>
#include <google/protobuf/stubs/hash.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/stubs/substitute.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {
    
string DotsToUnderscores(const string& name) {
    return StringReplace(name, ".", "_", true);
}
    
const char* const kKeywordList[] = {
    "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", "case",
    "catch", "char", "class", "compl", "const", "const_cast", "continue",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
    "explicit", "extern", "false", "float", "for", "friend", "goto", "if",
    "inline", "int", "long", "mutable", "namespace", "new", "not", "not_eq",
    "operator", "or", "or_eq", "private", "protected", "public", "register",
    "reinterpret_cast", "return", "short", "signed", "sizeof", "static",
    "static_cast", "struct", "switch", "template", "this", "throw", "true", "try",
    "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
    "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
};
    
hash_set<string> MakeKeywordsMap() {
    hash_set<string> result;
    for (int i = 0; i < GOOGLE_ARRAYSIZE(kKeywordList); i++) {
        result.insert(kKeywordList[i]);
    }
    return result;
}
    
hash_set<string> kKeywords = MakeKeywordsMap();

// Returns the file's package name.
string FilePackage(const FileDescriptor* file)
{
    string result;
    if (!file->package().empty())
    {
        result = file->package();
    }
    else if (file->options().has_java_package())
    {
        result = file->options().java_package();
    }
    return result;
}

// Returns output directory for the given package name.
string PackageToDir(string package_name)
{
    string package_dir =
    StringReplace(package_name, ".", "/", true);
    if (!package_dir.empty()) package_dir += "/";
    return package_dir;
}
    
string StripProto(const string& filename) {
    if (HasSuffixString(filename, ".protodevel")) {
        return StripSuffixString(filename, ".protodevel");
    } else {
        return StripSuffixString(filename, ".proto");
    }
}

string DependencyVarName(const FileDescriptor* file)
{
    string name = FilePackage(file);
    name = StringReplace(name, ".", "_", true);
    name += string("_") + StripProto(file->name());
    
    return name;
}
    
string FieldName(const FieldDescriptor* field) {
    string result = field->name();
    LowerString(&result);
    if (kKeywords.count(result) > 0) {
        result.append("_");
    }
    return result;
}
    
string ObjectFullName(const Descriptor* descriptor) {
    
    // Find "outer", the descriptor of the top-level message in which
    // "descriptor" is embedded.
//    const Descriptor* outer = descriptor;
//    while (outer->containing_type() != NULL) outer = outer->containing_type();
//    
//    const string& outer_name = outer->full_name();
    if (!descriptor->file()->package().empty()) {
        return descriptor->full_name().substr(descriptor->file()->package().size() + 1);
    }
    return descriptor->full_name();
}
    
string ObjectFullName(const EnumDescriptor* enum_descriptor) {
    if (enum_descriptor->containing_type() == NULL) {
        return enum_descriptor->name();
    } else {
        string result = ObjectFullName(enum_descriptor->containing_type());
        result += enum_descriptor->name();
        return result;
    }
}
    
string TSInterfaceFullName(const Descriptor* descriptor) {
    
    // Find "outer", the descriptor of the top-level message in which
    // "descriptor" is embedded.
    const Descriptor* outer = descriptor;
    while (outer->containing_type() != NULL) outer = outer->containing_type();
    
    const string& outer_name = outer->full_name();
    string inner_name = descriptor->full_name().substr(outer_name.size());
    
    return outer->name() + DotsToUnderscores(inner_name);
}
    
string DefaultValue(const FieldDescriptor* field) {
    switch (field->cpp_type()) {
        case FieldDescriptor::CPPTYPE_INT32:
            // gcc rejects the decimal form of kint32min and kint64min.
            if (field->default_value_int32() == kint32min) {
                // Make sure we are in a 2's complement system.
                GOOGLE_COMPILE_ASSERT(kint32min == -0x80000000, kint32min_value_error);
                return "-0x80000000";
            }
            return SimpleItoa(field->default_value_int32());
        case FieldDescriptor::CPPTYPE_UINT32:
            return SimpleItoa(field->default_value_uint32());
        case FieldDescriptor::CPPTYPE_INT64:
            // See the comments for CPPTYPE_INT32.
            if (field->default_value_int64() == kint64min) {
                // Make sure we are in a 2's complement system.
                GOOGLE_COMPILE_ASSERT(kint64min == GOOGLE_LONGLONG(-0x8000000000000000),
                                      kint64min_value_error);
                return "require(\"common/Int64\").MIN_VALUE";
            }
            return "require(\"common/Int64\").fromString(\"" + SimpleItoa(field->default_value_int64()) + "\")";
        case FieldDescriptor::CPPTYPE_UINT64:
            return "require(\"common/Int64\").fromString(\"" + SimpleItoa(field->default_value_uint64()) + "\", true)";
        case FieldDescriptor::CPPTYPE_DOUBLE: {
            double value = field->default_value_double();
            if (value == numeric_limits<double>::infinity()) {
                return "Number.POSITIVE_INFINITY";
            } else if (value == -numeric_limits<double>::infinity()) {
                return "Number.NEGATIVE_INFINITY";
            } else if (value != value) {
                return "NaN";
            } else {
                return SimpleDtoa(value);
            }
        }
        case FieldDescriptor::CPPTYPE_FLOAT:
        {
            float value = field->default_value_float();
            if (value == numeric_limits<float>::infinity()) {
                return "Number.POSITIVE_INFINITY";
            } else if (value == -numeric_limits<float>::infinity()) {
                return "Number.NEGATIVE_INFINITY";
            } else if (value != value) {
                return "NaN";
            } else {
                return SimpleFtoa(value);
            }
        }
        case FieldDescriptor::CPPTYPE_BOOL:
            return field->default_value_bool() ? "true" : "false";
        case FieldDescriptor::CPPTYPE_ENUM:
            // Lazy:  Generate a static_cast because we don't have a helper function
            //   that constructs the full name of an enum value.
            return SimpleItoa(field->default_value_enum()->number());
        case FieldDescriptor::CPPTYPE_STRING:
            return "\"" + CEscape(field->default_value_string()) +
            "\"";
        case FieldDescriptor::CPPTYPE_MESSAGE:
            // TODO not supported currently
            ;
    }
    // Can't actually get here; make compiler happy.  (We could add a default
    // case above but then we wouldn't get the nice compiler warning when a
    // new type is added.)
    GOOGLE_LOG(FATAL) << "Can't get here.";
    return "";
}

string DefaultValueByType(const FieldDescriptor* field)
{
    switch (field->cpp_type()) {
        case FieldDescriptor::CPPTYPE_INT32:
        case FieldDescriptor::CPPTYPE_UINT32:
            return "0";
        case FieldDescriptor::CPPTYPE_INT64:
            return "Int64.ZERO";
        case FieldDescriptor::CPPTYPE_UINT64:
            return "Int64.UZERO";
        case FieldDescriptor::CPPTYPE_DOUBLE:
        case FieldDescriptor::CPPTYPE_FLOAT:
            return "0";
        case FieldDescriptor::CPPTYPE_BOOL:
            return "false";
        case FieldDescriptor::CPPTYPE_ENUM:
            return "0";
        case FieldDescriptor::CPPTYPE_STRING:
            return "\"\"";
        case FieldDescriptor::CPPTYPE_MESSAGE:
            return "null";
    }
    // Can't actually get here; make compiler happy.  (We could add a default
    // case above but then we wouldn't get the nice compiler warning when a
    // new type is added.)
    GOOGLE_LOG(FATAL) << "Can't get here.";
    return "";
}
    
const char* GetCapitalizedType(const FieldDescriptor* field) {
    switch (field->type()) {
        case FieldDescriptor::TYPE_INT32   : return "Int32"   ;
        case FieldDescriptor::TYPE_UINT32  : return "UInt32"  ;
        case FieldDescriptor::TYPE_SINT32  : return "SInt32"  ;
        case FieldDescriptor::TYPE_FIXED32 : return "Fixed32" ;
        case FieldDescriptor::TYPE_SFIXED32: return "SFixed32";
        case FieldDescriptor::TYPE_INT64   : return "Int64"   ;
        case FieldDescriptor::TYPE_UINT64  : return "UInt64"  ;
        case FieldDescriptor::TYPE_SINT64  : return "SInt64"  ;
        case FieldDescriptor::TYPE_FIXED64 : return "Fixed64" ;
        case FieldDescriptor::TYPE_SFIXED64: return "SFixed64";
        case FieldDescriptor::TYPE_FLOAT   : return "Float"   ;
        case FieldDescriptor::TYPE_DOUBLE  : return "Double"  ;
        case FieldDescriptor::TYPE_BOOL    : return "Bool"    ;
        case FieldDescriptor::TYPE_STRING  : return "String"  ;
        case FieldDescriptor::TYPE_BYTES   : return "Bytes"   ;
        case FieldDescriptor::TYPE_ENUM    : return "Enum"    ;
        case FieldDescriptor::TYPE_GROUP   : return "Group"   ;
        case FieldDescriptor::TYPE_MESSAGE : return "Message" ;
            
            // No default because we want the compiler to complain if any new
            // types are added.
    }
    
    GOOGLE_LOG(FATAL) << "Can't get here.";
    return NULL;
}
    
JSVALType GetJSType(const FieldDescriptor* field)
{
    switch (field->type()) {
        case FieldDescriptor::TYPE_INT32:
        case FieldDescriptor::TYPE_UINT32:
        case FieldDescriptor::TYPE_SINT32:
        case FieldDescriptor::TYPE_FIXED32:
        case FieldDescriptor::TYPE_SFIXED32:
            return JSTYPE_INT;
            
        case FieldDescriptor::TYPE_INT64:
        case FieldDescriptor::TYPE_UINT64:
        case FieldDescriptor::TYPE_SINT64:
        case FieldDescriptor::TYPE_FIXED64:
        case FieldDescriptor::TYPE_SFIXED64:
            return JSTYPE_INT64;
            
        case FieldDescriptor::TYPE_FLOAT:
            return JSTYPE_FLOAT;
            
        case FieldDescriptor::TYPE_DOUBLE:
            return JSTYPE_DOUBLE;
            
        case FieldDescriptor::TYPE_BOOL:
            return JSTYPE_BOOLEAN;
            
        case FieldDescriptor::TYPE_STRING:
            return JSTYPE_STRING;
            
        case FieldDescriptor::TYPE_BYTES:
            return JSTYPE_BYTES;
            
        case FieldDescriptor::TYPE_ENUM:
            return JSTYPE_ENUM;
            
        case FieldDescriptor::TYPE_GROUP:
        case FieldDescriptor::TYPE_MESSAGE:
            return JSTYPE_MESSAGE;
            
            // No default because we want the compiler to complain if any new
            // types are added.
    }
    
    GOOGLE_LOG(FATAL) << "Can't get here.";
    return JSTYPE_INT;
}
    
    const char* GetTSTypeString(const FieldDescriptor* field)
    {
        switch (field->type()) {
            case FieldDescriptor::TYPE_INT32:
            case FieldDescriptor::TYPE_UINT32:
            case FieldDescriptor::TYPE_SINT32:
            case FieldDescriptor::TYPE_FIXED32:
            case FieldDescriptor::TYPE_SFIXED32:
            case FieldDescriptor::TYPE_INT64:
            case FieldDescriptor::TYPE_UINT64:
            case FieldDescriptor::TYPE_SINT64:
            case FieldDescriptor::TYPE_FIXED64:
            case FieldDescriptor::TYPE_SFIXED64:
            case FieldDescriptor::TYPE_FLOAT:
            case FieldDescriptor::TYPE_DOUBLE:
                return "number";
            case FieldDescriptor::TYPE_BOOL:
                return "boolean";
            case FieldDescriptor::TYPE_STRING:
                return "string";
                
            case FieldDescriptor::TYPE_BYTES:
                return "ArrayBuffer";
                
                // No default because we want the compiler to complain if any new
                // types are added.
        }
        
        return "";
    }
    
}
}
}
}
