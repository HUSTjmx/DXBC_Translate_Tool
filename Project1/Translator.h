// Todo
// ...
#pragma once
#include <string>
#include <vector>
#include "Singleton.h"

using namespace std;

// Assembly instruction.
// About detail info, please see https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/rsq--sm4---asm
// ...
enum class OpType {
	invalid,
	add,
	iadd,
	add_sat,
	mul,
	imul,
	mul_sat,
	div,
	mad,
	mad_sat,
	mov,
	movc,
	mov_sat,
	dp4,
	dp3,
	dp3_sat,
	dp2,
	ftou,
	ubfe,
	utof,
	round_ni,
	lt,
	frc,
	max,
	min,
	ge,
	rsq,
	itof,
	ret,
	sqrt,
	texture,
	texture_l,
	texture_d,
	texture_l_cube,
	texture3D,
	rcp,
	log,
	exp,
	ne,
	if_nz,
	Or,
	Else,
	Endif,
	deriv_rty_coarse,
	deriv_rtx_coarse,
	_and,
	eq,
	discard_nz,
	sample_b
};

template<OpType>
class OpTrait
{
public:
	static const int Flags[4];
};

//template<>
//class OpTrait<OpType::dp2>
//{
//	static const int Flags[4];
//};
template<>
const int OpTrait<OpType::dp2>::Flags[4] = { 1,1,0,0 };

template<>
const int OpTrait<OpType::dp3>::Flags[4] = { 1,1,1,0 };

template<>
const int OpTrait<OpType::dp3_sat>::Flags[4] = { 1,1,1,0 };

template<>
const int OpTrait<OpType::dp4>::Flags[4] = { 1,1,1,1 };


// ...
OpType StringToOp(const string& op);

// This class is used to translate dxbc code to hlsl code for reading.
// ...
class DXBC2HLSL : public Singleton<DXBC2HLSL>
{
public:

	// We must declare this parent class as a friend, 
	// because we need call it`s construct function that is private in the parent class.
	// ...
	friend class Singleton<DXBC2HLSL>;

	void Execute(string origin, string destination);

private:

	DXBC2HLSL() {}

	// If the end char of the word is ",", we should delete the last char.
	// ...
	string ModifyWord(const string& word);

	// Translate a line according to the sm4-asm op.
	// The inputs:
	// 	   <op> : The sm4-asm op type.
	// 	   <info> : the words of the instruction.
	// 	   <line> : the origin line that not split.
	// 	   <blank> : The number of blank at the start of the op.
	// ...
	string GetCodeByOp(OpType op, vector<string>& info, const string& line, const string::size_type blank);

	// Translate a line and make some modify.
	// ...
	string Translate(const string& line);

};


using Translator = DXBC2HLSL;

