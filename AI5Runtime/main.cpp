#include <iostream>

#include "StackMachine.h"
#include "Instruction.h"
#include "Variant.h"
#include <ctime>
#include <memory>

int main() 
{

	auto program = std::shared_ptr<std::vector<Instruction*>>(new std::vector<Instruction*>());
	auto statics = std::shared_ptr<std::vector<STATIC_DATA*>>(new std::vector<STATIC_DATA*>());


	/*
	char a = 0;
	char b = 1;
	__int64 c = 100000000;
	int nameindex = 33;
	int trueJump = 2;
	int jump = -8;	
	program->push_back(new Instruction(Instruction::CREATE_GLOBAL,&nameindex));
	program->push_back(new Instruction(Instruction::PUSH_SMALL_INTEGER,&a));
	program->push_back(new Instruction(Instruction::ASSIGN_NAME,&nameindex));
	program->push_back(new Instruction(Instruction::PUSH_NAME,&nameindex));
	program->push_back(new Instruction(Instruction::PUSH_SMALL_INTEGER,&b));
	program->push_back(new Instruction(Instruction::ADDITION,nullptr));
	program->push_back(new Instruction(Instruction::DOUBLE_TOP,nullptr));
	program->push_back(new Instruction(Instruction::ASSIGN_NAME,&nameindex));
	program->push_back(new Instruction(Instruction::PUSH_LARGE_INTEGER,&c));
	program->push_back(new Instruction(Instruction::GREATER,nullptr));
	program->push_back(new Instruction(Instruction::JUMP_LONG_RELATIVE_IF_TRUE,&trueJump));
	program->push_back(new Instruction(Instruction::JUMP_LONG_RELATIVE,&jump));
	program->push_back(new Instruction(Instruction::TERMINATE,nullptr));
	*/


	
	
	
	int a = 35;
	__int64 num = 12345;
	program->push_back(new Instruction(Instruction::CALL_NAME,&a));
	program->push_back(new Instruction(Instruction::TERMINATE,nullptr));
	program->push_back(new Instruction(Instruction::PUSH_LARGE_INTEGER,&num));
	program->push_back(new Instruction(Instruction::RET,nullptr));
	

	StackMachine sm(statics,program);

	sm.getName(35)->setGlobal(new UserFunctionVariant(2));

	clock_t begin = clock();

	sm.start();
	sm.getDataStack()->pop()->print();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Execution time: " << elapsed_secs << std::endl;

}