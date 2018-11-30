//
//  js_helpers.h
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#ifndef GOOGLE_PROTOBUF_COMPILER_JS_HELPERS_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_HELPERS_H__

#include <string>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {
    
string StripProto(const string& filename);

// Returns the file's package name.
string FilePackage(const FileDescriptor* file);
    
// Returns output directory for the given package name.
string PackageToDir(string package_name);
    
string DependencyVarName(const FileDescriptor* file);
    
string FieldName(const FieldDescriptor* field);
    
string ObjectFullName(const Descriptor* descriptor);
    
string ObjectFullName(const EnumDescriptor* enum_descriptor);
    
string TSInterfaceFullName(const Descriptor* descriptor);
    
string DefaultValue(const FieldDescriptor* field);
    
string DefaultValueByType(const FieldDescriptor* field);
    
const char* GetCapitalizedType(const FieldDescriptor* field);
    
const char* GetTSTypeString(const FieldDescriptor* field);
    
enum JSVALType {
    JSTYPE_INT,
    JSTYPE_INT64,
    JSTYPE_FLOAT,
    JSTYPE_DOUBLE,
    JSTYPE_BOOLEAN,
    JSTYPE_STRING,
    JSTYPE_BYTES,
    JSTYPE_ENUM,
    JSTYPE_MESSAGE
};

JSVALType GetJSType(const FieldDescriptor* field);

}
}
}
}

#endif /* defined(__protobuf__js_helpers__) */
