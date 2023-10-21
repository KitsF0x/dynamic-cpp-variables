#include <type_traits>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <assert.h>

/*
	Variable types enum
*/
enum class VariableType
{
	NULL_T,
	INT_T,
	DOUBLE_T,
	BOOL_T,
	STRING_T,
	OTHER_T
};
/*
	VariableType enum to string
*/
std::ostream& operator<<(std::ostream& os, const VariableType& variable)
{
	switch (variable)
	{
	case VariableType::NULL_T:
		os << "null";
		break;
	case VariableType::INT_T:
		os << "integer";
		break;
	case VariableType::DOUBLE_T:
		os << "double";
		break;
	case VariableType::BOOL_T:
		os << "bool";
		break;
	case VariableType::STRING_T:
		os << "string";
		break;
	case VariableType::OTHER_T:
		os << "other";
		break;
	}
	return os;
}
/*
	Variable class
*/
class Variable
{
public:
	void* data{ nullptr };
	int size{ 0 };
	VariableType type{ VariableType::NULL_T };

	std::string getValueAsString() const {
		std::stringstream ss;
		ss << "(" << size << ")";
		switch (type) {
		case VariableType::INT_T:
			ss << *reinterpret_cast<int*>(data);
			break;
		case VariableType::DOUBLE_T:
			ss << *reinterpret_cast<double*>(data);
			break;
		case VariableType::BOOL_T:
			ss << std::boolalpha << *reinterpret_cast<bool*>(data);
			break;
		case VariableType::STRING_T:
			ss << reinterpret_cast<char*>(data);
			break;
		case VariableType::OTHER_T:
			break;
		default:
			ss << data;
			break;
		}
		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Variable& variable)
	{
		std::string variableValue = variable.getValueAsString();
		if (variable.type == VariableType::STRING_T)
		{
			int indexAfterEndBracket = variableValue.find(')') + 1;

			variableValue.insert(variableValue.begin() + indexAfterEndBracket, '\"');
			variableValue.insert(variableValue.end(), '\"');
		}
		if (variable.type == VariableType::BOOL_T)
		{
			for (auto& el : variableValue)
			{
				el = toupper(el);
			}
		}
		if (variable.type == VariableType::NULL_T)
		{
			variableValue = "null";
		}
		return os << "[" << variable.getTypeAsString() << ", " << variableValue << "]";
	}

	std::string getTypeAsString() const
	{
		std::stringstream ss;
		ss << type;
		return ss.str();
	}
};

/*
	Main factory
*/
Variable make_variable(VariableType type, void* data, int size)
{
	Variable var;
	var.type = type;
	free(var.data);
	var.data = nullptr;
	var.size = 0;

	if (type != VariableType::NULL_T) {
		var.data = malloc(size);
		memcpy_s(var.data, size, data, size);
		var.size = size;
	}
	return var;
}
/*
	Factories for hardcoded types
*/
Variable make_variable_integral(long data)
{
	return make_variable(VariableType::INT_T, &data, sizeof(long));
}

Variable make_variable_double(double data)
{
	return make_variable(VariableType::DOUBLE_T, &data, sizeof(double));
}

Variable make_variable_bool(bool data)
{
	return make_variable(VariableType::BOOL_T, &data, sizeof(bool));
}

Variable make_variable_string(const char* data)
{
	return make_variable(VariableType::STRING_T, const_cast<char*>(data), sizeof(char) * (strlen(data) + 1));
}

Variable make_variable_other(void* data, int size)
{
	return make_variable(VariableType::OTHER_T, data, size);
}

Variable make_variable_null()
{
	return make_variable(VariableType::NULL_T, nullptr, 0);
}

int main() {
	/* null at start*/
	Variable var;
	std::cout << var << std::endl;
	/* integer */
	var = make_variable_integral(12);
	std::cout << var << std::endl;
	/* double */
	var = make_variable_double(12.34);
	std::cout << var << std::endl;
	/* string */
	var = make_variable_string("Hello, world!");
	std::cout << var << std::endl;
	/* bool */
	var = make_variable_bool(true);
	std::cout << var << std::endl;
	/* null made */
	var = make_variable_null();
	std::cout << var << std::endl;

	/* other/custom types */
	struct MyStruct
	{
		// 16 + 16 = 32
		// 32 / 8 =   4
		// 4 bytes in total
		std::uint16_t a;
		std::uint16_t b;
	};
	MyStruct myStruct{ 0xFF, 0x00 };
	var = make_variable_other(&myStruct, sizeof(MyStruct));
	std::cout << var << std::endl;

	// Value can be got
	MyStruct got = *reinterpret_cast<MyStruct*>(var.data);
	std::cout << "myStruct: " << myStruct.a << " " << myStruct.b << std::endl;
	std::cout << "got     : " << got.a << " " << got.b << std::endl;
}