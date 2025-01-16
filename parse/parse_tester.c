int main() {
    const char *input1 = "grep Hello | wc -l";
    const char *input2 = "grep Hello && wc -l";
    const char *input3 = "grep Hello | ls -la src/";
    const char *input4 = "ls -la src/ | wc -l";
    const char *input5 = "grep Hello | awk '{count++} END {print count}'";
    const char *input6 = "grep Hello | awk \"{count++} END {print count}\"";
    const char *input7 = "grep Hello | awk '\"{count++} END {print count}\"'";
    const char *input8 = "grep Hello | awk \"'{count++} END {print count}'\"";

    Token *tokens1 = tokenize(input1, &num_tokens);
    Token *tokens2 = tokenize(input2, &num_tokens);
}
