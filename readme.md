# Dynamic variables system fun in c++
## Example:
```cpp

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
```
## Output:
![img](https://raw.githubusercontent.com/KitsF0x/dynamic-cpp-variables/main/dynamicvars.png)
