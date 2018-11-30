//
//  js_field.h
//  protobuf
//
//  Created by zhao shuan on 8/3/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#ifndef GOOGLE_PROTOBUF_COMPILER_JS_FIELD_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_FIELD_H__

#include <string>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
namespace io {
    
class Printer;             // printer.h
    
}
}

namespace protobuf {
namespace compiler {
namespace js {
    
class FieldGenerator {
public:
    FieldGenerator() {}
    virtual ~FieldGenerator();
    
    virtual void GenerateMembers(io::Printer* printer) const = 0;
    virtual void GenerateFieldMethodDeclareTSD(io::Printer* printer) const = 0;
    virtual void GenerateFieldDeclareTSD(io::Printer* printer) const = 0;
    virtual void GenerateClearCode(io::Printer* printer) const = 0;
    virtual void GenerateCopyFromJSObjectCode(io::Printer* printer) const = 0;
    virtual void GenerateCopyToJSObjectCode(io::Printer* printer) const = 0;
    virtual void GenerateParsingCode(io::Printer* printer) const = 0;
    virtual void GenerateParsingCodeFromPacked(io::Printer* printer) const;
    virtual void GenerateSerializationCode(io::Printer* printer) const = 0;
    virtual void GenerateByteSizeCode(io::Printer* printer) const = 0;
private:
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FieldGenerator);
};
    
// Convenience class which constructs FieldGenerators for a Descriptor.
class FieldGeneratorMap {
public:
    explicit FieldGeneratorMap(const Descriptor* descriptor);
    ~FieldGeneratorMap();
    
    const FieldGenerator& get(const FieldDescriptor* field) const;
    
private:
    const Descriptor* descriptor_;
    scoped_array<scoped_ptr<FieldGenerator> > field_generators_;
    
    static FieldGenerator* MakeGenerator(const FieldDescriptor* field);
    
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FieldGeneratorMap);
};
    
}
}
}
}

#endif /* defined(__protobuf__js_field__) */
