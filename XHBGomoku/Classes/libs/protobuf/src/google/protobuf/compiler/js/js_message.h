//
//  js_message.h
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#ifndef GOOGLE_PROTOBUF_COMPILER_JS_MESSAGE_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_MESSAGE_H__

#include <string>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/compiler/js/js_field.h>

namespace google {
namespace protobuf {
namespace io {
    class Printer;             // printer.h
}
}

namespace protobuf {
namespace compiler {
namespace js {
    
class EnumGenerator;
    
class MessageGenerator {
public:
    // See generator.cc for the meaning of dllexport_decl.
    explicit MessageGenerator(const Descriptor* descriptor);
    
    ~MessageGenerator();
    
    // Generate definitions of all nested enums (must come before class
    // definitions because those classes use the enums definitions).
    void GenerateEnumDefinitions(io::Printer* printer);
    
    void GenerateMessageDefinition(io::Printer* printer);
    
    void GenerateNestedMessage(io::Printer* printer);
    
    void GenerateEnumDefinitionsTSD(io::Printer* printer);
    
    void GenerateMessageDefinitionTSD(io::Printer* printer);
    
    void GenerateClassDeclareTSD(io::Printer* printer);
    
    void GenerateInterfaceMessageTSD(io::Printer* printer);
    
private:
    void GenerateFields(io::Printer* printer);
    void GenerateFieldsTSD(io::Printer* printer);
    void GenerateByteSize(io::Printer* printer);
    void GenerateClear(io::Printer* printer);
    // TODO
//    void GenerateMergeFrom(io::Printer* printer);
//    void GenerateCopyFrom(io::Printer* printer);
    void GenerateCopyFromJSObject(io::Printer* printer);
    void GenerateCopyToJSObject(io::Printer* printer);
    
    void GenerateMergeFromCodedStream(io::Printer* printer);
    void GenerateSerializeToCodedStream(io::Printer* printer);
    
    scoped_array<scoped_ptr<MessageGenerator> > nested_generators_;
    scoped_array<scoped_ptr<EnumGenerator> > enum_generators_;
    FieldGeneratorMap field_generators_;
    
    const Descriptor* descriptor_;
    string fullname_;
};
    
}
}
}
}

#endif /* defined(__protobuf__js_message__) */
