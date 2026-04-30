/*
 * 配合 global_static_local_variables_demo.cpp 使用：
 * 在本文件中定义全局变量，在主 demo 中用 extern 声明后访问。
 *
 * 请勿在头文件中写这种“带定义”的全局变量；应头文件 extern 声明、某一个 .cpp 定义。
 */

int g_defined_in_other_translation_unit = 2026;
