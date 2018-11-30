//
//  js_string_field.h
//  protobuf
//
//  Created by zhao shuan on 8/3/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#ifndef GOOGLE_PROTOBUF_COMPILER_JS_STRING_FIELD_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_STRING_FIELD_H__

#include <map>
#include <string>
#include <google/protobuf/compiler/js/js_field.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {

class StringFieldGenerator : public FieldGenerator {
public:
    explicit StringFieldGenerator(const FieldDescriptor* descriptor);
    ~StringFieldGenerator();
    
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
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(StringFieldGenerator);
};

class RepeatedStringFieldGenerator : public FieldGenerator {
public:
    explicit RepeatedStringFieldGenerator(const FieldDescriptor* descriptor);
    ~RepeatedStringFieldGenerator();
    
    // implements FieldGenerator ---------------------------------------
    void GenerateMembers(io::Printer* printer) const;
    void GenerateFieldMethodDeclareTSD(io::Printer* printer) const;
    void GenerateFieldDeclareTSD(io::Printer* printer) const;
    void GenerateClearCode(io::Printer* printer) const;
    void GenerateCopyFromJSObjectCode(io::Printer* printer) const;
    void GenerateCopyToJSObjectCode(io::Printer* printer) const;
    void GenerateParsingCode(io::Printer* printer) const;
    void GenerateParsingCodeFromPacked(io::Printer* printer) const;
    void GenerateSerializationCode(io::Printer* printer) const;
    void GenerateByteSizeCode(io::Printer* printer) const;
    
private:
    const FieldDescriptor* descriptor_;
    map<string, string> variables_;
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RepeatedStringFieldGenerator);
};

}  // namespace js
}  // namespace compiler
}  // namespace protobuf

}  // namespace google
#endif
