class Test
{
public:
    int hehe;
    float nima;

private:
    double you __attribute__((annotate("func")));

public:
    int test(float value);
}__attribute__((annotate("enable")));