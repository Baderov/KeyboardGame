#ifndef SOURCE_H
#define SOURCE_H

#include "Console.h"
#include "Database.h"
#include "GameVariable.h"
#include "GameWindow.h"
#include "Page.h"

void setVariable(std::unique_ptr<GameVariable>& gv, std::unique_ptr<GameWindow>& gw, std::unique_ptr<Database>& database);

void resetVariable(std::unique_ptr<GameVariable>& gv, std::unique_ptr<Database>& database);

int main();

#endif
