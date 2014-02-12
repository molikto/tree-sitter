#include "parser.h"
#include <ctype.h>

enum ts_symbol {
    ts_symbol_array,
    ts_symbol_number,
    ts_symbol_object,
    ts_symbol_string,
    ts_symbol_value,
    ts_aux_end,
    ts_aux_repeat_helper1,
    ts_aux_repeat_helper2,
    ts_aux_token1,
    ts_aux_token2,
    ts_aux_token3,
    ts_aux_token4,
    ts_aux_token5,
    ts_aux_token6,
    ts_aux_token7,
};

static const char *ts_symbol_names[] = {
    "array",
    "number",
    "object",
    "string",
    "value",
    "end",
    "repeat_helper1",
    "repeat_helper2",
    "token1",
    "token2",
    "token3",
    "token4",
    "token5",
    "token6",
    "token7",
};

static void ts_lex(TSParser *parser) {
    START_LEXER();
    switch (LEX_STATE()) {
        case 0:
            if (LOOKAHEAD_CHAR() == '\0')
                ADVANCE(1);
            LEX_ERROR(1, EXPECT({"<EOF>"}));
        case 1:
            ACCEPT_TOKEN(ts_aux_end);
        case 2:
            if (LOOKAHEAD_CHAR() == ',')
                ADVANCE(3);
            ACCEPT_TOKEN(ts_aux_token3);
        case 3:
            ACCEPT_TOKEN(ts_aux_token2);
        case 4:
            if (LOOKAHEAD_CHAR() == ']')
                ADVANCE(5);
            LEX_ERROR(1, EXPECT({"]"}));
        case 5:
            ACCEPT_TOKEN(ts_aux_token4);
        case 6:
            if (LOOKAHEAD_CHAR() == ',')
                ADVANCE(3);
            if (LOOKAHEAD_CHAR() == ']')
                ADVANCE(5);
            LEX_ERROR(2, EXPECT({",", "]"}));
        case 7:
            if (LOOKAHEAD_CHAR() == '}')
                ADVANCE(8);
            LEX_ERROR(1, EXPECT({"}"}));
        case 8:
            ACCEPT_TOKEN(ts_aux_token7);
        case 9:
            if (LOOKAHEAD_CHAR() == ',')
                ADVANCE(3);
            if (LOOKAHEAD_CHAR() == '}')
                ADVANCE(8);
            LEX_ERROR(2, EXPECT({",", "}"}));
        case 10:
            if (LOOKAHEAD_CHAR() == '\"')
                ADVANCE(11);
            if ('0' <= LOOKAHEAD_CHAR() && LOOKAHEAD_CHAR() <= '9')
                ADVANCE(17);
            if (LOOKAHEAD_CHAR() == '[')
                ADVANCE(18);
            if (LOOKAHEAD_CHAR() == '{')
                ADVANCE(19);
            LEX_ERROR(4, EXPECT({"\"", "0-9", "[", "{"}));
        case 11:
            if (!((LOOKAHEAD_CHAR() == '\"') ||
                (LOOKAHEAD_CHAR() == '\\')))
                ADVANCE(12);
            if (LOOKAHEAD_CHAR() == '\\')
                ADVANCE(14);
            if (']' <= LOOKAHEAD_CHAR() && LOOKAHEAD_CHAR() <= '\\')
                ADVANCE(16);
            LEX_ERROR(2, EXPECT({"<EOF>-!", "#-<MAX>"}));
        case 12:
            if (!((LOOKAHEAD_CHAR() == '\"') ||
                (LOOKAHEAD_CHAR() == '\\')))
                ADVANCE(12);
            if (LOOKAHEAD_CHAR() == '\"')
                ADVANCE(13);
            if (LOOKAHEAD_CHAR() == '\\')
                ADVANCE(14);
            if (']' <= LOOKAHEAD_CHAR() && LOOKAHEAD_CHAR() <= '\\')
                ADVANCE(16);
            LEX_ERROR(1, EXPECT({"<ANY>"}));
        case 13:
            ACCEPT_TOKEN(ts_symbol_string);
        case 14:
            if (!((LOOKAHEAD_CHAR() == '\"') ||
                (LOOKAHEAD_CHAR() == '\\')))
                ADVANCE(12);
            if (LOOKAHEAD_CHAR() == '\"')
                ADVANCE(15);
            if ('#' <= LOOKAHEAD_CHAR() && LOOKAHEAD_CHAR() <= '\"')
                ADVANCE(12);
            if (LOOKAHEAD_CHAR() == '\\')
                ADVANCE(14);
            if (']' <= LOOKAHEAD_CHAR() && LOOKAHEAD_CHAR() <= '\\')
                ADVANCE(16);
            LEX_ERROR(1, EXPECT({"<ANY>"}));
        case 15:
            if (!((LOOKAHEAD_CHAR() == '\"') ||
                (LOOKAHEAD_CHAR() == '\\')))
                ADVANCE(12);
            if (LOOKAHEAD_CHAR() == '\"')
                ADVANCE(13);
            if (LOOKAHEAD_CHAR() == '\\')
                ADVANCE(14);
            if (']' <= LOOKAHEAD_CHAR() && LOOKAHEAD_CHAR() <= '\\')
                ADVANCE(16);
            ACCEPT_TOKEN(ts_symbol_string);
        case 16:
            if (LOOKAHEAD_CHAR() == '\"')
                ADVANCE(12);
            LEX_ERROR(1, EXPECT({"\""}));
        case 17:
            if ('0' <= LOOKAHEAD_CHAR() && LOOKAHEAD_CHAR() <= '9')
                ADVANCE(17);
            ACCEPT_TOKEN(ts_symbol_number);
        case 18:
            ACCEPT_TOKEN(ts_aux_token1);
        case 19:
            ACCEPT_TOKEN(ts_aux_token5);
        case 20:
            if (LOOKAHEAD_CHAR() == ':')
                ADVANCE(21);
            LEX_ERROR(1, EXPECT({":"}));
        case 21:
            ACCEPT_TOKEN(ts_aux_token6);
        case 22:
            if (LOOKAHEAD_CHAR() == '\"')
                ADVANCE(11);
            LEX_ERROR(1, EXPECT({"\""}));
        default:
            LEX_PANIC();
    }
    FINISH_LEXER();
}

static TSParseResult ts_parse(const char *input) {
    START_PARSER();
    switch (PARSE_STATE()) {
        case 0:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(1);
                case ts_symbol_number:
                    SHIFT(1);
                case ts_symbol_object:
                    SHIFT(1);
                case ts_symbol_string:
                    SHIFT(1);
                case ts_symbol_value:
                    SHIFT(2);
                case ts_aux_token1:
                    SHIFT(3);
                case ts_aux_token5:
                    SHIFT(48);
                default:
                    PARSE_PANIC();
            }
        case 1:
            SET_LEX_STATE(0);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_end:
                    REDUCE(ts_symbol_value, 1, COLLAPSE({0}));
                default:
                    PARSE_PANIC();
            }
        case 2:
            SET_LEX_STATE(0);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_end:
                    ACCEPT_INPUT();
                default:
                    PARSE_PANIC();
            }
        case 3:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(5);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 4:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_value, 1, COLLAPSE({0}));
                case ts_aux_token3:
                    REDUCE(ts_symbol_value, 1, COLLAPSE({0}));
                default:
                    PARSE_PANIC();
            }
        case 5:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper2:
                    SHIFT(6);
                case ts_aux_token2:
                    SHIFT(8);
                case ts_aux_token3:
                    SHIFT(6);
                default:
                    PARSE_PANIC();
            }
        case 6:
            SET_LEX_STATE(4);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token4:
                    SHIFT(7);
                default:
                    PARSE_PANIC();
            }
        case 7:
            SET_LEX_STATE(0);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_end:
                    REDUCE(ts_symbol_array, 4, COLLAPSE({1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        case 8:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(9);
                case ts_symbol_number:
                    SHIFT(9);
                case ts_symbol_object:
                    SHIFT(9);
                case ts_symbol_string:
                    SHIFT(9);
                case ts_symbol_value:
                    SHIFT(10);
                case ts_aux_token1:
                    SHIFT(12);
                case ts_aux_token5:
                    SHIFT(42);
                default:
                    PARSE_PANIC();
            }
        case 9:
            SET_LEX_STATE(6);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_value, 1, COLLAPSE({0}));
                case ts_aux_token4:
                    REDUCE(ts_symbol_value, 1, COLLAPSE({0}));
                default:
                    PARSE_PANIC();
            }
        case 10:
            SET_LEX_STATE(6);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper2:
                    SHIFT(11);
                case ts_aux_token2:
                    SHIFT(8);
                case ts_aux_token4:
                    REDUCE(ts_aux_repeat_helper2, 2, COLLAPSE({1, 0}));
                default:
                    PARSE_PANIC();
            }
        case 11:
            SET_LEX_STATE(4);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token4:
                    REDUCE(ts_aux_repeat_helper2, 3, COLLAPSE({1, 0, 1}));
                default:
                    PARSE_PANIC();
            }
        case 12:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(13);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 13:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper2:
                    SHIFT(14);
                case ts_aux_token2:
                    SHIFT(8);
                case ts_aux_token3:
                    SHIFT(14);
                default:
                    PARSE_PANIC();
            }
        case 14:
            SET_LEX_STATE(4);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token4:
                    SHIFT(15);
                default:
                    PARSE_PANIC();
            }
        case 15:
            SET_LEX_STATE(6);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_array, 4, COLLAPSE({1, 0, 1, 1}));
                case ts_aux_token4:
                    REDUCE(ts_symbol_array, 4, COLLAPSE({1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        case 16:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(17);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 17:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper2:
                    SHIFT(18);
                case ts_aux_token2:
                    SHIFT(8);
                case ts_aux_token3:
                    SHIFT(18);
                default:
                    PARSE_PANIC();
            }
        case 18:
            SET_LEX_STATE(4);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token4:
                    SHIFT(19);
                default:
                    PARSE_PANIC();
            }
        case 19:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_array, 4, COLLAPSE({1, 0, 1, 1}));
                case ts_aux_token3:
                    REDUCE(ts_symbol_array, 4, COLLAPSE({1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        case 20:
            SET_LEX_STATE(22);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_string:
                    SHIFT(21);
                default:
                    PARSE_PANIC();
            }
        case 21:
            SET_LEX_STATE(20);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token6:
                    SHIFT(22);
                default:
                    PARSE_PANIC();
            }
        case 22:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(23);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 23:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper1:
                    SHIFT(24);
                case ts_aux_token2:
                    SHIFT(26);
                case ts_aux_token3:
                    SHIFT(24);
                default:
                    PARSE_PANIC();
            }
        case 24:
            SET_LEX_STATE(7);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token7:
                    SHIFT(25);
                default:
                    PARSE_PANIC();
            }
        case 25:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_object, 6, COLLAPSE({1, 0, 1, 0, 1, 1}));
                case ts_aux_token3:
                    REDUCE(ts_symbol_object, 6, COLLAPSE({1, 0, 1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        case 26:
            SET_LEX_STATE(22);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_string:
                    SHIFT(27);
                default:
                    PARSE_PANIC();
            }
        case 27:
            SET_LEX_STATE(20);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token6:
                    SHIFT(28);
                default:
                    PARSE_PANIC();
            }
        case 28:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(29);
                case ts_symbol_number:
                    SHIFT(29);
                case ts_symbol_object:
                    SHIFT(29);
                case ts_symbol_string:
                    SHIFT(29);
                case ts_symbol_value:
                    SHIFT(30);
                case ts_aux_token1:
                    SHIFT(32);
                case ts_aux_token5:
                    SHIFT(36);
                default:
                    PARSE_PANIC();
            }
        case 29:
            SET_LEX_STATE(9);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_value, 1, COLLAPSE({0}));
                case ts_aux_token7:
                    REDUCE(ts_symbol_value, 1, COLLAPSE({0}));
                default:
                    PARSE_PANIC();
            }
        case 30:
            SET_LEX_STATE(9);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper1:
                    SHIFT(31);
                case ts_aux_token2:
                    SHIFT(26);
                case ts_aux_token7:
                    REDUCE(ts_aux_repeat_helper1, 4, COLLAPSE({1, 0, 1, 0}));
                default:
                    PARSE_PANIC();
            }
        case 31:
            SET_LEX_STATE(7);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token7:
                    REDUCE(ts_aux_repeat_helper1, 5, COLLAPSE({1, 0, 1, 0, 1}));
                default:
                    PARSE_PANIC();
            }
        case 32:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(33);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 33:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper2:
                    SHIFT(34);
                case ts_aux_token2:
                    SHIFT(8);
                case ts_aux_token3:
                    SHIFT(34);
                default:
                    PARSE_PANIC();
            }
        case 34:
            SET_LEX_STATE(4);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token4:
                    SHIFT(35);
                default:
                    PARSE_PANIC();
            }
        case 35:
            SET_LEX_STATE(9);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_array, 4, COLLAPSE({1, 0, 1, 1}));
                case ts_aux_token7:
                    REDUCE(ts_symbol_array, 4, COLLAPSE({1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        case 36:
            SET_LEX_STATE(22);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_string:
                    SHIFT(37);
                default:
                    PARSE_PANIC();
            }
        case 37:
            SET_LEX_STATE(20);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token6:
                    SHIFT(38);
                default:
                    PARSE_PANIC();
            }
        case 38:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(39);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 39:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper1:
                    SHIFT(40);
                case ts_aux_token2:
                    SHIFT(26);
                case ts_aux_token3:
                    SHIFT(40);
                default:
                    PARSE_PANIC();
            }
        case 40:
            SET_LEX_STATE(7);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token7:
                    SHIFT(41);
                default:
                    PARSE_PANIC();
            }
        case 41:
            SET_LEX_STATE(9);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_object, 6, COLLAPSE({1, 0, 1, 0, 1, 1}));
                case ts_aux_token7:
                    REDUCE(ts_symbol_object, 6, COLLAPSE({1, 0, 1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        case 42:
            SET_LEX_STATE(22);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_string:
                    SHIFT(43);
                default:
                    PARSE_PANIC();
            }
        case 43:
            SET_LEX_STATE(20);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token6:
                    SHIFT(44);
                default:
                    PARSE_PANIC();
            }
        case 44:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(45);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 45:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper1:
                    SHIFT(46);
                case ts_aux_token2:
                    SHIFT(26);
                case ts_aux_token3:
                    SHIFT(46);
                default:
                    PARSE_PANIC();
            }
        case 46:
            SET_LEX_STATE(7);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token7:
                    SHIFT(47);
                default:
                    PARSE_PANIC();
            }
        case 47:
            SET_LEX_STATE(6);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token2:
                    REDUCE(ts_symbol_object, 6, COLLAPSE({1, 0, 1, 0, 1, 1}));
                case ts_aux_token4:
                    REDUCE(ts_symbol_object, 6, COLLAPSE({1, 0, 1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        case 48:
            SET_LEX_STATE(22);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_string:
                    SHIFT(49);
                default:
                    PARSE_PANIC();
            }
        case 49:
            SET_LEX_STATE(20);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token6:
                    SHIFT(50);
                default:
                    PARSE_PANIC();
            }
        case 50:
            SET_LEX_STATE(10);
            switch (LOOKAHEAD_SYM()) {
                case ts_symbol_array:
                    SHIFT(4);
                case ts_symbol_number:
                    SHIFT(4);
                case ts_symbol_object:
                    SHIFT(4);
                case ts_symbol_string:
                    SHIFT(4);
                case ts_symbol_value:
                    SHIFT(51);
                case ts_aux_token1:
                    SHIFT(16);
                case ts_aux_token5:
                    SHIFT(20);
                default:
                    PARSE_PANIC();
            }
        case 51:
            SET_LEX_STATE(2);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_repeat_helper1:
                    SHIFT(52);
                case ts_aux_token2:
                    SHIFT(26);
                case ts_aux_token3:
                    SHIFT(52);
                default:
                    PARSE_PANIC();
            }
        case 52:
            SET_LEX_STATE(7);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_token7:
                    SHIFT(53);
                default:
                    PARSE_PANIC();
            }
        case 53:
            SET_LEX_STATE(0);
            switch (LOOKAHEAD_SYM()) {
                case ts_aux_end:
                    REDUCE(ts_symbol_object, 6, COLLAPSE({1, 0, 1, 0, 1, 1}));
                default:
                    PARSE_PANIC();
            }
        default:
            PARSE_PANIC();
    }
    FINISH_PARSER();
}

TSParseConfig ts_parse_config_json = {
    .parse_fn = ts_parse,
    .symbol_names = ts_symbol_names
};