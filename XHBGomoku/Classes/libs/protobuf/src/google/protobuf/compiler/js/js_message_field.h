//
//  js_message_field.h
//  protobuf
//
//  Created by zhao shuan on 8/3/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#ifndef GOOGLE_PROTOBUF_COMPILER_JS_MESSAGE_FIELD_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_MESSAGE_FIELD_H__

#include <map>
#include <string>
#include <google/protobuf/compiler/js/js_field.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {

class MessageFieldGenerator : public FieldGenerator {
public:
    explicit MessageFieldGenerator(const FieldDescriptor* descriptor);
    ~MessageFieldGenerator();
    
    // implements FieldGenerator ---------------------------------------
    void GenerateMembers(io::Printer* printer) const;
    void GenerateFieldMethodDeclareTSD(io::Printer* printer) const;
    void GenerateFieldDeclareTSD(io::Printer* printer) const;
    void GenerateClearCode(io::Printer* printer) const;
    void GenerateCopyFromJSObjectCode(io::Printer* printer) const;
    void GenerateCopyToJSObjectCode(io::Printer* printer) const;
    void GenerateParsingCode(io::Printer* printer) const;
    void GenerateSerializationCode(io::Printer* printer) const;
    void GenerateByteSizeCode(io::Printer* printer) const;
    
private:
    const FieldDescriptor* descriptor_;
    map<string, string> variables_;
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(MessageFieldGenerator);
};

class RepeatedMessageFieldGenerator : public FieldGenerator {
public:
    explicit RepeatedMessageFieldGenerator(const FieldDescriptor* descriptor);
    ~RepeatedMessageFieldGenerator();
    
    // implements FieldGenerator ---------------------------------------
    void GenerateMembers(io::Printer* printer) const;
    void GenerateFieldMethodDeclareTSD(io::Printer* printer) const;
    void GenerateFieldDeclareTSD(io::Printer* printer) const;
    void GenerateClearCode(io::Printer* printer) const;
    void GenerateCopyFromJSObjectCode(io::Printer* printer) const;
    void GenerateCopyToJSObjectCode(io::Printer* printer) const;
    void GenerateParsingCode(io::Printer* printer) const;
    void GenerateSerializationCode(io::Printer* printer) const;
    void GenerateByteSizeCode(io::Printer* printer) const;
    
private:
    const FieldDescriptor* descriptor_;
    map<string, string> variables_;
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RepeatedMessageFieldGenerator);
};

}  // namespace js
}  // namespace compiler
}  // namespace protobuf

}  // namespace google
#endif
