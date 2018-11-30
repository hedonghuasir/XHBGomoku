//
//  js_enum.cpp
//  protobuf
//
//  Created by zhao shuan on 7/29/15.
//  Copyright (c) 2015 hoolai. All rights reserved.
//

#include "js_enum.h"

#include <google/protobuf/compiler/js/js_enum.h>
#include <google/protobuf/compiler/js/js_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace js {

EnumGenerator::EnumGenerator(const EnumDescriptor* descriptor) : descriptor_(descriptor)
{
    fullname_ = ObjectFullName(descriptor);
}

EnumGenerator::~EnumGenerator()
{
    
}

void EnumGenerator::Generate(io::Printer* printer)
{
    printer->Print("exports.$name$ = {\n", "name", fullname_);
    printer->Indent();
    for (int i = 0; i < descriptor_->value_count(); i++) {
        const EnumValueDescriptor* value = descriptor_->value(i);
        printer->Print("get $name$() {return $value$;},\n", "name", value->name(), "value", SimpleItoa(value->number()));
    }
    printer->Outdent();
    printer->Print("};\n");
    printer->Print("\n");
}
    
void EnumGenerator::GenerateTSD(io::Printer* printer)
{
    printer->Print("const enum $name$ {\n", "name", fullname_);
    printer->Indent();
    for (int i = 0; i < descriptor_->value_count(); i++) {
        const EnumValueDescriptor* value = descriptor_->value(i);
        printer->Print("$name$ = $value$,\n", "name", value->name(), "value", SimpleItoa(value->number()));
    }
    printer->Outdent();
    printer->Print("}\n");
    printer->Print("\n");
}
    
}
}
}
}