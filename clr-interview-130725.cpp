//  有两个色子，一个是正常的，六面分别1-6的数字；另一个六面都是空白的。现在有0-6的数字，请给出一个方案，
//  将0-6中的任意数字涂在空白的色子上，使得当同时扔两个色子时，以相等的概率出现某一个数字（这个数字是两
//  个色子上数的和），比如，如果一个色子是1，另一个色子是2，则出现的数字是3。


//这个题目相当于是构造一个新的服从均匀分布的随机变量，这个新的随机变量的一部分是服从均匀随机变量的。即

//Y = X + Z; X是已知的随机变量，Y是要求的随机变量，X，Y都是服从均匀分布的，求Z

// 第二个色子的六个面全部涂上相同的数字，或者3面涂0，3面涂6