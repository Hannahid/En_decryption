#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<string.h>
#include <math.h>
#include<algorithm>
using namespace std;
typedef long long ll;
int e, d, n;

int gcd(int a, int b)  //求最大公约数
{
    int c = 0;
    if(a<b) swap(a,b);
    c = b;
    do
    {
        b = c;
        c = a%b;
        a = b;
    }
    while (c != 0);
    return b;
}

int PrimarityTest(int a, int i) //判断i是否是素数
{
    int flag=0;
    for(a;a<i;a++)
    {
        if(i%a==0)
        {
            flag=1;
            break;
        }
    }
    if(flag) return 0;
    return 1;
    // complete this part
}

int ModularExponention(int a, int b, int n)  //求a^bmodn
{
    int y;

    /*使用二进制平方乘法计算 pow(a,b) % n*/
    y=1;

    while(b != 0)
    {
        /*对于b中的每个1，累加y*/

        if(b & 1)
            y = (y*a) % n;

        /*对于b中的每一位，计算a的平方*/
        a = (a*a) % n;

        /*准备b中的下一位*/
        b = b>>1;
    }

    return y;
    // complete this part
}

void extgcd(ll a,ll b,ll& d,ll& x,ll& y) //获取(1/a)modb得结果
{
    if(!b)
    {
        d=a;
        x=1;
        y=0;
    }
    else
    {
        extgcd(b,a%b,d,y,x);
        y-=x*(a/b);
    }
}

int ModularInverse(int a,int b)  //获取(1/a)modb得结果
{
    ll d,x,y;
    extgcd(a,b,d,x,y);
    return d==1?(x+b)%b:-1;
    // complete this part
}

void KeyGeneration()  //获取公钥密钥
{
    int p, q;
    int phi_n;

    do
    {
        do
            p = rand();
        while (p % 2 == 0);

    }
    while (!PrimarityTest(2, p));

    do
    {
        do
            q = rand();
        while (q % 2 == 0);
    }
    while (!PrimarityTest(2, q));

    n = p * q;
    phi_n = (p - 1) * (q - 1);

    do
        e = rand() % (phi_n - 2) + 2; // 1 < e < phi_n
    while (gcd(e, phi_n) != 1);

    d = ModularInverse(e,phi_n);
}

void Encryption(int value, FILE* out)  //加密
{
    int cipher;
    cipher = ModularExponention(value, e, n);
    fprintf(out, "%d ", cipher);
}

void Decryption(int value, FILE* out)  //解密
{
    int decipher;
    decipher = ModularExponention(value, d, n);
    fprintf(out, "%c", decipher);
}
int main(void)
{
    FILE* inp, * out;
    char filepath[15], filename[100];

    strcpy(filepath, "F:\Desktop\\");  //文件路径

    sprintf(filename, "%s%s", filepath, "cipher.txt");
    out = fopen(filename, "w+");  //打开文件
    fclose(out);
    sprintf(filename, "%s%s", filepath, "decipher.txt");
    out = fopen(filename, "w+");  //打开文件
    fclose(out);

    KeyGeneration();  //获取公钥密钥

    sprintf(filename, "%s%s", filepath, "plain.txt");
    inp = fopen(filename, "r+");  //读取原文件
    if (inp == NULL)
    {
        printf("Error opening Source File.\n");
        exit(1);
    }

    sprintf(filename, "%s%s", filepath, "cipher.txt");
    out = fopen(filename, "w+");
    if (out == NULL)
    {
        printf("Error opening Destination File.\n");
        exit(1);
    }

    // encryption starts
    while (1)
    {
        char ch = getc(inp);  //读取文件字符，一个字符一个字符的读取输出
        if (ch == -1)
            break;
        int value = toascii(ch);  //toascii将字符转化成对应ascall值
        Encryption(value, out);  //加密输出
    }

    fclose(inp);
    fclose(out);

    // decryption starts
    sprintf(filename, "%s%s", filepath, "cipher.txt");
    inp = fopen(filename, "r");
    if (inp == NULL)
    {
        printf("Error opening Cipher Text.\n");
        exit(1);
    }

    sprintf(filename, "%s%s", filepath, "decipher.txt");
    out = fopen(filename, "w+");
    if (out == NULL)
    {
        printf("Error opening File.\n");
        exit(1);
    }

    while (1)
    {
        int cip;
        if (fscanf(inp, "%d", &cip) == -1)
            break;
        Decryption(cip, out);  //解密
    }
    fclose(out);

    return 0;
}
