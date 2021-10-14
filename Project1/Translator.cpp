#include "Translator.h"
#include "Symbol.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

static int allFlags[4] = { 1, 1, 1, 1 };


bool HasDot(OpType op)
{
	return op == OpType::dp2 || op == OpType::dp3 || op == OpType::dp4 || op == OpType::dp3_sat;
}

void SolveInfo(vector<string>& info, OpType op)
{
	if (info.size() <= 0)return;

	// Load info to symbol.
	// ...
	vector<Symbol> symbols;
	auto func = [&symbols](const string& s)
	{
		Symbol a(s);
		symbols.push_back(a);
	};
	std::for_each(info.begin(), info.end(), func);

	// Transport read array
	// ...
	symbols[0].ComputeWriteChannels();
	for (auto s = symbols.begin() + 1;s != symbols.end();++s)
	{
		if (HasDot(op)) {
			switch (op)
			{
			
			case OpType::dp4:
				s->SetWritedFlag(const_cast<int*>(OpTrait<OpType::dp4>::Flags));
				break;
			case OpType::dp3:
				s->SetWritedFlag(const_cast<int*>(OpTrait<OpType::dp3>::Flags));
				break;
			case OpType::dp3_sat:
				s->SetWritedFlag(const_cast<int*>(OpTrait<OpType::dp3_sat>::Flags));
				break;
			case OpType::dp2:
				s->SetWritedFlag(const_cast<int*>(OpTrait<OpType::dp2>::Flags));
				break;
			default:
				break;
			}
		}
		else s->SetWritedFlag(symbols[0].IsWrited);
	}



	// Get final symbol.
	// ...
	for (int i = 0;i < info.size();++i)
	{
		info[i] = symbols[i].GetName();
	}
}

OpType StringToOp(const string& op) {
	if (!op.compare("add"))
		return OpType::add;
	if (!op.compare("iadd"))
		return OpType::iadd;
	if (!op.compare("add_sat"))
		return OpType::add_sat;
	if (!op.compare("mul"))
		return OpType::mul;
	if (!op.compare("imul"))
		return OpType::imul;
	if (!op.compare("mul_sat"))
		return OpType::mul_sat;
	if (!op.compare("div"))
		return OpType::div;
	if (!op.compare("mad"))
		return OpType::mad;
	if (!op.compare("mad_sat"))
		return OpType::mad_sat;
	if (!op.compare("mov"))
		return OpType::mov;
	if (!op.compare("movc"))
		return OpType::movc;
	if (!op.compare("mov_sat"))
		return OpType::mov_sat;
	if (!op.compare("dp4"))
		return OpType::dp4;
	if (!op.compare("dp3"))
		return OpType::dp3;
	if (!op.compare("dp3_sat"))
		return OpType::dp3_sat;
	if (!op.compare("dp2"))
		return OpType::dp2;
	if (!op.compare("ftou"))
		return OpType::ftou;
	if (!op.compare("ubfe"))
		return OpType::ubfe;
	if (!op.compare("utof"))
		return OpType::utof;
	if (!op.compare("round_ni"))
		return OpType::round_ni;
	if (!op.compare("lt"))
		return OpType::lt;
	if (!op.compare("frc"))
		return OpType::frc;
	if (!op.compare("max"))
		return OpType::max;
	if (!op.compare("min"))
		return OpType::min;
	if (!op.compare("ge"))
		return OpType::ge;
	if (!op.compare("rsq"))
		return OpType::rsq;
	if (!op.compare("itof"))
		return OpType::itof;
	if (!op.compare("sqrt"))
		return OpType::sqrt;
	if (!op.compare("sample_indexable(texture2d)(float,float,float,float)"))
		return OpType::texture;
	if (!op.compare("sample_l_indexable(texture2d)(float,float,float,float)"))
		return OpType::texture_l;
	if (!op.compare("sample_l_indexable(texturecube)(float,float,float,float)"))
		return OpType::texture_l_cube;
	if (!op.compare("sample_d_indexable(texture2d)(float,float,float,float)"))
		return OpType::texture_d;
	if (!op.compare("sample_indexable(texture3d)(float,float,float,float)"))
		return OpType::texture3D;
	if (!op.compare("rcp"))
		return OpType::rcp;
	if (!op.compare("log"))
		return OpType::log;
	if (!op.compare("exp"))
		return OpType::exp;
	if (!op.compare("ne"))
		return OpType::ne;
	if (!op.compare("if_nz"))
		return OpType::if_nz;
	if (!op.compare("or"))
		return OpType::Or;
	if (!op.compare("else"))
		return OpType::Else;
	if (!op.compare("endif"))
		return OpType::Endif;
	if (!op.compare("deriv_rty_coarse"))
		return OpType::deriv_rty_coarse;
	if (!op.compare("deriv_rtx_coarse"))
		return OpType::deriv_rtx_coarse;
	if (!op.compare("discard_nz"))
		return OpType::discard_nz;
	if (!op.compare("sample_b(texture2d)(float,float,float,float)"))
		return OpType::sample_b;
	if (!op.compare("eq"))
		return OpType::eq;
	if (!op.compare("and"))
		return OpType::_and;
	if (!op.compare("ret"))
		return OpType::ret;
	// //todo
	else
		cout << op << " is undefined op" << endl;
	return OpType::invalid;
}

void DXBC2HLSL::Execute(string origin, string destination)
{
	ifstream dxbc(origin, ifstream::in);
	ofstream hlsl(destination, ofstream::out);
	string line;
	while (getline(dxbc, line)) {
		hlsl << Translate(line);
		hlsl << std::endl;
	}
	dxbc.close();
	hlsl.close();
}

string DXBC2HLSL::ModifyWord(const string& word)
{
	if (word[word.size() - 1] == ',')
	{
		return word.substr(0, word.size() - 1);
	}
	return word;
}

string DXBC2HLSL::GetCodeByOp(OpType op, vector<string>& info, const string& line, const string::size_type blank)
{
	string hlsl = "Failed to translate:" + line;
	string blankStr = "";
	for (string::size_type i = 0; i < blank; i++)
	{
		blankStr += " ";
	}

	if (op == OpType::invalid)return blankStr + hlsl;

	SolveInfo(info, op);
	switch (op) {
	case OpType::dp2:
	case OpType::dp3:														//dot
	case OpType::dp4:
		hlsl = info[0] + " =  clamp(dot(" + info[1] + " , " + info[2] + "), 0.0f, 1.0f);";
		break;
	case OpType::dp3_sat:
		hlsl = info[0] + " = dot(" + info[1] + " , " + info[2] + ");";
		break;
	case OpType::iadd:
	case OpType::add:
		hlsl = info[0] + " = " + info[1] + " + " + info[2] + ";";
		break;
	case OpType::add_sat:
		hlsl = info[0] + " = clamp(" + info[1] + " + " + info[2] + ", 0.0f, 1.0f);";
		break;
	case OpType::imul:
	case OpType::mul:
		hlsl = info[0] + " = " + info[1] + " * " + info[2] + ";";
		break;
	case OpType::mul_sat:
		hlsl = info[0] + " = clamp(" + info[1] + " * " + info[2] + ", 0.0f, 1.0f);";
		break;
	case OpType::mad:
		hlsl = info[0] + " = " + info[1] + " * " + info[2] + " + " + info[3] + ";";
		break;
	case OpType::mad_sat:
		hlsl = info[0] + " = clamp(" + info[1] + " * " + info[2] + " + " + info[3] + ", 0.0f, 1.0f);";
		break;
	case OpType::div:
		hlsl = info[0] + " = " + info[1] + " / " + info[2] + ";";
		break;
	case OpType::movc:
		hlsl = info[0] + " = " + info[1] + "? " + info[2] + " : " + info[3] + ";";
		break;
	case OpType::mov:
		hlsl = info[0] + " = " + info[1] + ";";
		break;
	case OpType::mov_sat:
		hlsl = info[0] + " = clamp(" + info[1] + ", 0.0f, 1.0f);";
		break;
	case OpType::ftou:														//int
		hlsl = info[0] + " = abs(int(" + info[1] + "));";
		break;
	case OpType::ubfe:														//float
		hlsl = info[0] + " = float(" + info[1] + ");";
		break;
	case OpType::round_ni:													//floor
		hlsl = info[0] + " = floor(" + info[1] + ");";
		break;
	case OpType::lt:														//<
		hlsl = info[0] + " = " + info[1] + " < " + info[2] + ";";
		break;
	case OpType::frc:														//fract
		hlsl = info[0] + " = fract(" + info[1] + ");";
		break;
	case OpType::max:
		hlsl = info[0] + " = max(" + info[1] + " , " + info[2] + ");";
		break;
	case OpType::min:
		hlsl = info[0] + " = min(" + info[1] + " , " + info[2] + ");";
		break;
	case OpType::ge:
		hlsl = info[0] + " = " + info[1] + " >= " + info[2] + ";";
		break;
	case OpType::rsq:
		hlsl = info[0] + " = 1.0f / sqrt(" + info[1] + ");";
		break;
	case OpType::sqrt:
		hlsl = info[0] + " = sqrt(" + info[1] + ");";
		break;
	case OpType::itof:
	case OpType::utof:
		hlsl = info[0] + " = float(" + info[1] + ");";
		break;
	case OpType::texture:
	case OpType::texture_l:
	case OpType::texture_d:
		hlsl = info[0] + " = tex2D(" + info[3] + ", " + info[1] + ".xy); //texture object:" + info[2];
		break;
	case OpType::texture3D:
		hlsl = info[0] + " = tex3D(" + info[3] + ", " + info[1] + ".xy); //texture object:" + info[2];
		break;
	case OpType::texture_l_cube:
		hlsl = info[0] + " = texCUBE(" + info[3] + ", " + info[1] + ".xy); //texture object:" + info[2];
		break;
	case OpType::log:
		hlsl = info[0] + " = log(" + info[1] + ");";
		break;
	case OpType::exp:
		hlsl = info[0] + " = exp(" + info[1] + ");";
		break;
	case OpType::rcp:
		hlsl = info[0] + " = 1.0f / " + info[1] + ";";
		break;
	case  OpType::ne:
		hlsl = info[0] + " = (" + info[1] + " != " + info[2] + ");";
		break;
	case  OpType::if_nz:
		hlsl = "if( " + info[0] + "== 0 ) {";
		break;
	case OpType::Or:
		hlsl = info[0] + " = " + info[1] + " | " + info[2] + ";";
		break;
	case OpType::Else:
		hlsl = "}else{";
		break;
	case OpType::Endif:
		hlsl = "}";
		break;
	case OpType::deriv_rtx_coarse:
		hlsl = "Computes the rate of change of components: " + info[0] + " = rate_x( " + info[1] + " );";
		break;
	case OpType::deriv_rty_coarse:
		hlsl = "Computes the rate of change of components: " + info[0] + " = rate_y( " + info[1] + " );";
		break;
	case OpType::discard_nz:
		hlsl = "if(" + info[0] + " != 0) { discard; }";
		break;
	case OpType::eq:
		hlsl = info[0] + " = " + info[1] + " == " + info[2] + ";";
		break;
	case OpType::sample_b:
		hlsl = info[0] + " = " + info[2] + ".SampleLevel(" + info[3] + ", " + info[1] + ".xy, " + info[4] + ");";
		break;
	case OpType::_and:
		hlsl = info[0] + " = " + info[1] + " & " + info[2] + ";";
		break;
	case OpType::ret:
		hlsl = "return;";
		break;
	}
	hlsl = blankStr + hlsl;
	return hlsl;
}

string DXBC2HLSL::Translate(const string& line)
{
	string op_s, word, next_word;
	istringstream code(line);
	code >> op_s;

	string::size_type blank = line.find(op_s, 0);

	OpType op = StringToOp(op_s);

	//
	vector<string> info;
	if (code >> word)
		while (1) {
			if (code >> next_word) {
				//0.111 or -0.1111
				if (IsNum(next_word[0]) || (IsMathOp(next_word[0]) && next_word.size() > 1 && IsNum(next_word[1]))) {
					word = word + next_word;
				}
				else if (IsMathOp(next_word[0]) && next_word.size() == 1) {
					word = word + next_word;
					code >> next_word;
					word = word + next_word;
				}
				else {
					info.push_back(ModifyWord(word));
					word = next_word;
				}
			}
			else
			{
				info.push_back(ModifyWord(word));
				break;
			}
		}

	//L() to vec()
	for (int i = 0; i < info.size();i++)
	{
		if (info[i].find("l(") != info[i].npos)
		{
			int num = 0;
			for (int j = 0;j < info[i].size();j++)
				if (info[i][j] == ',')num++;
			if (num == 0)
			{
				info[i].replace(info[i].begin(), info[i].begin() + 2, "");
				info[i].replace(info[i].end() - 1, info[i].end(), "");
			}
			if (num == 3)
				info[i].replace(info[i].begin(), info[i].begin() + 2, "vec4(");
		}
	}

	return GetCodeByOp(op, info, line, blank);
}
