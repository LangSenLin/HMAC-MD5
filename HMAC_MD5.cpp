#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;

string toBinary(int n, int m) {
    string temp = "";
    int* a = (int*)malloc(m * sizeof(int));
    int i = 0;
    for (; n > 0; n /= 2) {
        a[i++] = n % 2;
    }
    for (int j = i - 1; j >= 0; j--) {
        if (a[j] == 0) {
            temp += '0';
        }
        else {
            temp += '1';
        }
    }
    string temp1 = "";
    if (temp.length() < m) {
        for (int j = 0; j < m - temp.length(); j++) {
            temp1 += '0';
        }
        temp1 += temp;
    }
    free(a);
    return temp1;
}

int* getX() {
    int* temp = (int*)malloc(64 * sizeof(int));
    for (int i = 0; i < 16; i++) {
        temp[i] = i;
    }
    for (int i = 16; i < 32; i++) {
        temp[i] = (temp[i - 16] * 5 + 1) % 16;
    }
    for (int i = 32; i < 48; i++) {
        temp[i] = (temp[i - 32] * 3 + 5) % 16;
    }
    for (int i = 48; i < 64; i++) {
        temp[i] = (temp[i - 48] * 7 + 0) % 16;
    }
    return temp;
}


long long toLong(string str) {
    long long num = 0;
    int size = str.length();
    for (int i = 0; i < size; i++) {
        if (str[i] == '1') {
            num += pow(2, 32 - i);
        }
    }
    return num;
}

long long rol(long long val, int size){
    long long res = val << size;
    res |= val >> (32 - size);
    return res;
}

long long F(long long B, long long C, long long D) {
    return ((B & C) | ((~B) & D));
}

long long G(long long B, long long C, long long D) {
    return ((B & D) | (C & (~D)));
}

long long H(long long B, long long C, long long D) {
    return (B ^ C ^ D);
}

long long I(long long B, long long C, long long D) {
    return (C ^ (B | (~D)));
}

long long FA(long long A, long long B, long long C, long long D, string str, long long T, int s) {
    long long X = toLong(str);
    long long x = A + F(B, C, D) + X + T;
    return rol(x, s) + B;
}

long long GA(long long A, long long B, long long C, long long D, string str, long long T, int s) {
    long long X = toLong(str);
    long long x = A + G(B, C, D) + X + T;
    return rol(x, s) + B;
}

long long HA(long long A, long long B, long long C, long long D, string str, long long T, int s) {
    long long X = toLong(str);
    long long x = A + H(B, C, D) + X + T;
    return rol(x, s) + B;
}

long long IA(long long A, long long B, long long C, long long D, string str, long long T, int s) {
    long long X = toLong(str);
    long long x = A + I(B, C, D) + X + T;
    return rol(x, s) + B;
}

void fourLoop(string mSubStr, long long& A, long long& B, long long& C, long long& D) {
    long long a = A,
        b = B,
        c = C,
        d = D;
    string group[16];
    //Message div to 16 groups
    for (int i = 0; i < 16; i++) {
        group[i] = mSubStr.substr(i * 32, 32);
    }
    //选择消息分组X初始化
    int* X = getX();
    //T表初始化
    long long T[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
    //移位长度初始化
    int s[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
    //F迭代运算
    for (int i = 0; i < 4; i++) {
        a = FA(a, b, c, d, group[X[i * 4 + 0]], T[i * 4 + 0], s[i * 4 + 0]);
        b = FA(b, c, d, a, group[X[i * 4 + 1]], T[i * 4 + 1], s[i * 4 + 1]);
        c = FA(c, d, a, b, group[X[i * 4 + 2]], T[i * 4 + 2], s[i * 4 + 2]);
        d = FA(d, a, b, c, group[X[i * 4 + 3]], T[i * 4 + 3], s[i * 4 + 3]);
    }
    //G迭代运算
    for (int i = 4; i < 8; i++) {
        a = GA(a, b, c, d, group[X[i * 4 + 0]], T[i * 4 + 0], s[i * 4 + 0]);
        b = GA(b, c, d, a, group[X[i * 4 + 1]], T[i * 4 + 1], s[i * 4 + 1]);
        c = GA(c, d, a, b, group[X[i * 4 + 2]], T[i * 4 + 2], s[i * 4 + 2]);
        d = GA(d, a, b, c, group[X[i * 4 + 3]], T[i * 4 + 3], s[i * 4 + 3]);
    }
    //H迭代运算
    for (int i = 8; i < 12; i++) {
        a = HA(a, b, c, d, group[X[i * 4 + 0]], T[i * 4 + 0], s[i * 4 + 0]);
        b = HA(b, c, d, a, group[X[i * 4 + 1]], T[i * 4 + 1], s[i * 4 + 1]);
        c = HA(c, d, a, b, group[X[i * 4 + 2]], T[i * 4 + 2], s[i * 4 + 2]);
        d = HA(d, a, b, c, group[X[i * 4 + 3]], T[i * 4 + 3], s[i * 4 + 3]);
    }
    //I迭代运算
    for (int i = 12; i < 16; i++) {
        a = IA(a, b, c, d, group[X[i * 4 + 0]], T[i * 4 + 0], s[i * 4 + 0]);
        b = IA(b, c, d, a, group[X[i * 4 + 1]], T[i * 4 + 1], s[i * 4 + 1]);
        c = IA(c, d, a, b, group[X[i * 4 + 2]], T[i * 4 + 2], s[i * 4 + 2]);
        d = IA(d, a, b, c, group[X[i * 4 + 3]], T[i * 4 + 3], s[i * 4 + 3]);
    }
    A += a;
    B += b;
    C += c;
    D += d;
    //用来做类似归一化
    //在上面分组消息转变成long long类型可能存在错误，使得过程中A、B、C、D的值变得过大
    //故归一化
    A = A & 0xFFFFFFFFL;
    B = B & 0xFFFFFFFFL;
    C = C & 0xFFFFFFFFL;
    D = D & 0xFFFFFFFFL;
}

string MD5(string m, int times) {
    string M = "";
    //transform string to bits
    for (int i = 0; i < m.length(); i++) {
        int temp = (int)m[i];
        M += toBinary(temp, 8);
    }
    int l = M.length();
    //padding
    int modR = M.length() % 512;
    if (modR != 448) {
        M += '1';
        int paddingN = 0;
        if (modR > 448) {
            paddingN = 2 * 512 - modR - 64 - 1;
        }
        else {
            paddingN = 512 - modR - 64 - 1;
        }
        for (int i = 0; i < paddingN; i++) {
            M += '0';
        }
        //pad length
        string lengthOfM = toBinary(l, 64);
        M += lengthOfM;
    }
    //padding over

    //四轮循环开始
    int numOfBlock = M.length() / 512;
    long long A = 0X67452301L,
        B = 0XEFCDAB89L,
        C = 0X98BADCFEL,
        D = 0X10325476L;
    for (int i = 0; i < numOfBlock; i++) {
        string mSubStr = M.substr(i * 512, 512);
        fourLoop(mSubStr, A, B, C, D);
    }

    A = abs(A);
    B = abs(B);
    C = abs(C);
    D = abs(D);
    if (times == 1) {
        cout << "第" << times << "次MD5结果为：" << endl;
    }
    else {
        cout << "最终结果为:" << endl;
    }
    
    cout << "        " << hex << A;
    cout << hex << B;
    cout << hex << C;
    cout << hex << D << endl << endl;
    string AA = to_string(A);
    string BB = to_string(B);
    string CC = to_string(C);
    string DD = to_string(D);
    return AA + BB + CC + DD;
}

string XOR(string key, string s) {
    string temp = "";
    for (int i = 0; i < 64; i++) {
        temp += key[i] ^ s[i];
    }
    return temp;
}

int getNum(string t) {
    int temp = 0;
    if (t[0] == '1') {
        temp += pow(2, 7);
    }
    if (t[1] == '1') {
        temp += pow(2, 6);
    }
    if (t[2] == '1') {
        temp += pow(2, 5);
    }
    if (t[3] == '1') {
        temp += pow(2, 4);
    }
    if (t[4] == '1') {
        temp += pow(2, 3);
    }
    if (t[5] == '1') {
        temp += pow(2, 2);
    }
    if (t[6] == '1') {
        temp += pow(2, 1);
    }
    if (t[7] == '1') {
        temp += 1;
    }
    return temp;
}

string bitToHex(string result) {
    string temp = "";
    string table = "0123456789ABCDEF";
    int size = result.length() / 4;
    int num;
    for (int i = 0; i < size; i++) {
        num = 0;
        if (result[i * 4 + 0] == '1') {
            num += pow(2, 3);
        }
        if (result[i * 4 + 1] == '1') {
            num += pow(2, 2);
        }
        if (result[i * 4 + 2] == '1') {
            num += pow(2, 1);
        }
        if (result[i * 4 + 3] == '1') {
            num += pow(2, 0);
        }
        temp += table[num];
    }
    return temp;
}

int main() {
    /*
    1)在签名密钥（key）后面添加0来创建一个长为64字节的字符串(str)；
    2)将上一步生成的字符串(str)与ipad(0x36)做异或运算，形成结果字符串(istr)；
    3)将消息内容data附加到第二步的结果字符串(istr)的末尾；
    4)做md5运算于第三步生成的数据流(istr)；
    5)将第一步生成的字符串(str)与opad(0x5c)做异或运算，形成结果字符串(ostr)；
    6)再将第四步的结果(istr)附加到第五步的结果字符串(ostr)的末尾；
    7)做md5运算于第六步生成的数据流(ostr)，输出最终结果(out)。
    */
    string m = "1234567890qwertyuiopasdfghjklzxcvbndjsaifbewbgwbgnekjdnfkjs ndjfbidbgbdsnfsndibnsdkbfkdsbfksdbfbsdifbsdbfsdbfmqdsahjugtwfmoqegnrongmfpodkpkwpjdonsbderhgifcnsxklnmfoeirfuesnalnooo";
    cout << "被运算的消息为：" << m << endl << endl;
    string key = "123456789qwer";
    cout << "加密的密钥为：" << key << endl << endl;
    for (int i = key.length(); i < 64; i++) {
        key += '0';
    }
    
    string ipad = "", opad = "";
    for (int i = 0; i < 64; i += 4) {
        ipad += "0x36";
        opad += "0x5c";
    }
    cout << "密钥以及ipad以及opad的填充结果为：" << endl;
    cout << "key:" << key << endl;
    cout << "ipad:" << ipad << endl;
    cout << "opad:" << opad << endl << endl;

    string istr, ostr;
    istr = XOR(key, ipad);
    cout << "密钥和ipad的异或结果为：" << istr << endl << endl;
    istr += m;
    m = istr;
    string result1 = MD5(m, 1);



    ostr = XOR(key, opad);
    cout << "密钥和opad的异或结果为：" << ostr << endl << endl;
    ostr += result1;
    m = ostr;
    string result2 = MD5(m, 2);
    
    return 0;
}


