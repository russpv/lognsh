int main(void) {
    UNITY_BEGIN();
    RUN_TEST_GROUP(parser_basic);
    RUN_TEST_GROUP(parser_operators);
    RUN_TEST_GROUP(parser_errors);


    return UNITY_END();
}