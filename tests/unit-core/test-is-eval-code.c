/* Copyright JS Foundation and other contributors, http://js.foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"
#include "jerryscript.h"

#include "test-common.h"

static jerry_value_t
check_eval (const jerry_call_info_t *call_info_p, /**< call information */
            const jerry_value_t args_p[], /**< arguments list */
            const jerry_length_t args_cnt) /**< arguments length */
{
  JERRY_UNUSED (call_info_p);

  TEST_ASSERT (args_cnt == 2 && jerry_is_eval_code (args_p[0]) == jerry_value_is_true (args_p[1]));
  return jerry_create_boolean (true);
} /* check_eval */

static void
test_parse (const char *source_p, /**< source code */
            jerry_parse_options_t *options_p) /**< options passed to jerry_parse */
{
  jerry_value_t parse_result = jerry_parse ((const jerry_char_t *) source_p, strlen (source_p), options_p);
  TEST_ASSERT (!jerry_value_is_error (parse_result));
  TEST_ASSERT (!jerry_is_eval_code (parse_result));

  jerry_value_t result;

  if (options_p->options & JERRY_PARSE_HAS_ARGUMENT_LIST)
  {
    jerry_value_t this_value = jerry_create_undefined ();
    result = jerry_call_function (parse_result, this_value, NULL, 0);
    jerry_release_value (this_value);
  }
  else if (options_p->options & JERRY_PARSE_MODULE)
  {
    result = jerry_module_link (parse_result, NULL, NULL);
    TEST_ASSERT (!jerry_value_is_error (result));
    jerry_release_value (result);
    result = jerry_module_evaluate (parse_result);
  }
  else
  {
    result = jerry_run (parse_result);
  }

  TEST_ASSERT (!jerry_value_is_error (result));

  jerry_release_value (parse_result);
  jerry_release_value (result);
} /* test_parse */

int
main (void)
{
  TEST_INIT ();

  jerry_init (JERRY_INIT_EMPTY);

  jerry_value_t global_object_value = jerry_get_global_object ();

  jerry_value_t function_value = jerry_create_external_function (check_eval);
  jerry_value_t function_name_value = jerry_create_string ((const jerry_char_t *) "check_eval");
  jerry_release_value (jerry_set_property (global_object_value, function_name_value, function_value));

  jerry_release_value (function_name_value);
  jerry_release_value (function_value);
  jerry_release_value (global_object_value);

  jerry_parse_options_t parse_options;
  const char *source_p = TEST_STRING_LITERAL ("eval('check_eval(function() {}, true)')\n"
                                              "check_eval(function() {}, false)");

  parse_options.options = JERRY_PARSE_NO_OPTS;
  test_parse (source_p, &parse_options);

  if (jerry_is_feature_enabled (JERRY_FEATURE_MODULE))
  {
    parse_options.options = JERRY_PARSE_MODULE;
    test_parse (source_p, &parse_options);
  }

  parse_options.options = JERRY_PARSE_HAS_ARGUMENT_LIST;
  parse_options.argument_list = jerry_create_string ((const jerry_char_t *) "");
  test_parse (source_p, &parse_options);
  jerry_release_value (parse_options.argument_list);

  parse_options.options = JERRY_PARSE_NO_OPTS;
  source_p = TEST_STRING_LITERAL ("check_eval(new Function('a', 'return a'), true)");
  test_parse (source_p, &parse_options);

  source_p = TEST_STRING_LITERAL ("check_eval(function() {}, true)");
  jerry_release_value (jerry_eval ((const jerry_char_t *) source_p, strlen (source_p), JERRY_PARSE_NO_OPTS));

  jerry_cleanup ();
  return 0;
} /* main */
