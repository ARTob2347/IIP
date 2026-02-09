#pragma once
#include "form_model.h"
#include <string>
void save( const SimpleForm form, const std::string save_name);
SimpleForm load(SimpleForm form, const std::string save_name);