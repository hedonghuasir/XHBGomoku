//
//  js_file.cpp
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include <google/protobuf/compiler/js/js_file.h>
#include <google/protobuf/compiler/js/js_helpers.h>
#include <google/protobuf/compiler/js/js_message.h>
#include <google/protobuf/compiler/js/js_enum.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {

    FileGenerator::FileGenerator(const FileDescriptor* file)
    : file_(file),
      message_generators_(new scoped_ptr<MessageGenerator>[file->message_type_count()]),
      enum_generators_(new scoped_ptr<EnumGenerator>[file->enum_type_count()])
{
    file_package_ = FilePackage(file);
    
    for (int i = 0; i < file->message_type_count(); i++) {
        message_generators_[i].reset(new MessageGenerator(file->message_type(i)));
    }
    
    for (int i = 0; i < file->enum_type_count(); i++) {
        enum_generators_[i].reset(new EnumGenerator(file->enum_type(i)));
    }
}

FileGenerator::~FileGenerator() {}
    
void FileGenerator::GenerateSource(io::Printer* printer)
{
    printer->Print("var WireFormat = require(\"protobuf/WireFormat\");\n");
    printer->Print("var CodedInputStream = require(\"protobuf/CodedInputStream\");\n");
    printer->Print("var CodedOutputStream = require(\"protobuf/CodedOutputStream\");\n");
    printer->Print("var Int64 = require(\"common/Int64\");\n");
    printer->Print("\n");
    printer->Print("var $name$ = exports;\n", "name", DependencyVarName(file_));
    for (int i = 0; i < file_->dependency_count(); i++) {
        printer->Print(
                       "var $dependencyvar$ = require(\"$dependencydir$$dependencyname$.pb.js\");\n",
                       "dependencyvar", DependencyVarName(file_->dependency(i)),
                       "dependencydir", PackageToDir(FilePackage(file_->dependency(i))),
                       "dependencyname", StripProto(file_->dependency(i)->name()));
    }
    printer->Print("\n");
    for (int i = 0; i < file_->enum_type_count(); i++) {
        enum_generators_[i]->Generate(printer);
    }
    for (int i = 0; i < file_->message_type_count(); i++) {
        message_generators_[i]->GenerateMessageDefinition(printer);
        message_generators_[i]->GenerateEnumDefinitions(printer);
        message_generators_[i]->GenerateNestedMessage(printer);
    }
}
    
void FileGenerator::GenerateTSDSource(io::Printer* printer)
{
    printer->Print("import {Message} from \"protobuf/protobuf.d\";\n");
    for (int i = 0; i < file_->dependency_count(); i++) {
        printer->Print(
                       "import $dependencyvar$ = require(\"$dependencydir$$dependencyname$.pb.d\");\n",
                       "dependencyvar", DependencyVarName(file_->dependency(i)),
                       "dependencydir", PackageToDir(FilePackage(file_->dependency(i))),
                       "dependencyname", StripProto(file_->dependency(i)->name()));
    }
    printer->Print("\n");
    printer->Print("declare namespace $name$ {\n", "name", DependencyVarName(file_));
    for (int i = 0; i < file_->enum_type_count(); i++) {
        enum_generators_[i]->GenerateTSD(printer);
    }
    for (int i = 0; i < file_->message_type_count(); i++) {
        message_generators_[i]->GenerateMessageDefinitionTSD(printer);
    }
    printer->Print("}\n");
    printer->Print("export = $name$;\n", "name", DependencyVarName(file_));
}
    
}
}
}
}
