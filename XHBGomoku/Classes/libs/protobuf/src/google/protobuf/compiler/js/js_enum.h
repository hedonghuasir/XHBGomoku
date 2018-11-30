#ifndef GOOGLE_PROTOBUF_COMPILER_JS_ENUM_H__
#define GOOGLE_PROTOBUF_COMPILER_JS_ENUM_H__

#include <string>
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
                
                class EnumGenerator {
                public:
                    explicit EnumGenerator(const EnumDescriptor* descriptor);
                    ~EnumGenerator();
                    
                    void Generate(io::Printer* printer);
                    void GenerateTSD(io::Printer* printer);
                    
                private:
                    const EnumDescriptor* descriptor_;
                    string fullname_;
                };
                
            }
        }
    }
}

#endif