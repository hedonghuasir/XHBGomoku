#ifndef GOOGLE_PROTOBUF_COMPILER_JS_ENUM_FIELD_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_ENUM_FIELD_H__

#include <map>
#include <string>
#include <google/protobuf/compiler/js/js_field.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {

class EnumFieldGenerator : public FieldGenerator {
public:
    explicit EnumFieldGenerator(const FieldDescriptor* descriptor);
    ~EnumFieldGenerator();
    
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
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(EnumFieldGenerator);
};

class RepeatedEnumFieldGenerator : public FieldGenerator {
public:
    explicit RepeatedEnumFieldGenerator(const FieldDescriptor* descriptor);
    ~RepeatedEnumFieldGenerator();
    
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
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RepeatedEnumFieldGenerator);
};

}  // namespace js
}  // namespace compiler
}  // namespace protobuf

}  // namespace google
#endif