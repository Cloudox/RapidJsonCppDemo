#include <iostream>
#include "rapidjson/document.h"
#include <vector>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace std;
using namespace rapidjson;

// 教程： http://rapidjson.org/zh-cn/md_doc_tutorial_8zh-cn.html

int main() {
    // json字符串
    string jsonStr = "{\"name\": \"Cloudox\", \"age\": 18}";

    // 转成json对象
    char* json = (char*)jsonStr.c_str();
    Document document;
    document.Parse(json);
    cout << "Hello World, I'm " << document["name"].GetString() << ", age " << document["age"].GetInt() <<  endl;
    
    // 添加字符串值的节点
    Value str;
    str = "male";
    document.AddMember("gender", str, document.GetAllocator());// 参数：key、value

    // 添加double节点
    Value doub;
    doub =  188.8;
    document.AddMember("height", doub, document.GetAllocator());

    cout << "my gender is " << document["gender"].GetString() << " and height is " << document["height"].GetDouble() <<  endl;

    // 添加数组
    Value arr(kArrayType);
    Document::AllocatorType& allocator = document.GetAllocator();
    vector<string> vec = {"read", "code", "movie", "game", "walk"};
    for (int i = 0; i < 5; i++) {
        // 这里很奇怪的是直接放vec[i]编译不通过，不得不转char*再转StringRef
        arr.PushBack(StringRef(vec[i].c_str()), allocator);   // 可能需要调用 realloc() 所以需要 allocator
    }
    // arr.PushBack("read", allocator).PushBack("code", allocator); // 另一种组装数组的方式
    document.AddMember("hobby", arr, allocator);
    
    cout << "my hobby:" << endl;
    // 使用引用来连续访问，方便之余还更高效。
    const Value& a = document["hobby"];
    assert(a.IsArray());// 所有类型的值都可以先进行判断
    for (SizeType i = 0; i < a.Size(); i++) // PS: Array 与 std::vector 相似，除了使用索引，也可使用迭代器来访问所有元素。
        cout << a[i].GetString() << endl;

    // 添加一个json子对象
    Value uni(kObjectType);
    uni.AddMember("name", "HUST", allocator);
    uni.AddMember("location", "wuhan", allocator);
    document.AddMember("university", uni, allocator);

    cout << "My university is " << document["university"]["name"].GetString() << " which locate in " << document["university"]["location"].GetString() << endl;

    // 转成字符串
    StringBuffer strBuffer;  
    Writer<StringBuffer> writer(strBuffer);  
    document.Accept(writer);  
    cout << strBuffer.GetString() << endl;
    
    system("pause");// 暂停以显示终端窗口
    return 0;
}