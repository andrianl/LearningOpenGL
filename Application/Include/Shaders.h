#pragma once
#include <string>
#include <Platform.h>

uint32 CreateShader(const std::string& VertexShader, const std::string& PixelShader);
uint32 CompileShader(const std::string& Source, uint32 Type);