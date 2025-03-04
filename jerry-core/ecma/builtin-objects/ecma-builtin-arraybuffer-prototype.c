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

#include "ecma-builtin-helpers.h"
#include "ecma-builtins.h"
#include "ecma-conversion.h"
#include "ecma-exceptions.h"
#include "ecma-function-object.h"
#include "ecma-gc.h"
#include "ecma-globals.h"
#include "ecma-helpers.h"
#include "ecma-objects.h"
#include "ecma-arraybuffer-object.h"
#include "jrt.h"
#include "jrt-libc-includes.h"

#if JERRY_BUILTIN_TYPEDARRAY

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-arraybuffer-prototype.inc.h"
#define BUILTIN_UNDERSCORED_ID arraybuffer_prototype
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup arraybufferprototype ECMA ArrayBuffer.prototype object built-in
 * @{
 */

/**
 * The ArrayBuffer.prototype.bytelength accessor
 *
 * See also:
 *          ES2015, 24.1.4.1
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
static ecma_value_t
ecma_builtin_arraybuffer_prototype_bytelength_getter (ecma_value_t this_arg) /**< this argument */
{
  if (ecma_is_value_object (this_arg))
  {
    ecma_object_t *object_p = ecma_get_object_from_value (this_arg);

    if (ecma_object_class_is (object_p, ECMA_OBJECT_CLASS_ARRAY_BUFFER))
    {
      if (ecma_arraybuffer_is_detached (object_p))
      {
        return ecma_raise_type_error (ECMA_ERR_MSG (ecma_error_arraybuffer_is_detached));
      }
      uint32_t len = ecma_arraybuffer_get_length (object_p);

      return ecma_make_uint32_value (len);
    }
  }

  return ecma_raise_type_error (ECMA_ERR_MSG ("Argument 'this' is not a ArrayBuffer object"));
} /* ecma_builtin_arraybuffer_prototype_bytelength_getter */

/**
 * The ArrayBuffer.prototype object's 'slice' routine
 *
 * See also:
 *          ECMA-262 v11, 24.1.4.3
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
static ecma_value_t
ecma_builtin_arraybuffer_prototype_object_slice (ecma_value_t this_arg, /**< this argument */
                                                 const ecma_value_t *argument_list_p, /**< arguments list */
                                                 uint32_t arguments_number) /**< number of arguments */
{
  if (!ecma_is_value_object (this_arg))
  {
    return ecma_raise_type_error (ECMA_ERR_MSG ("Argument 'this' is not an object"));
  }

  ecma_object_t *object_p = ecma_get_object_from_value (this_arg);

  /* 2. */
  if (!ecma_object_class_is (object_p, ECMA_OBJECT_CLASS_ARRAY_BUFFER))
  {
    return ecma_raise_type_error (ECMA_ERR_MSG ("Argument 'this' is not an ArrayBuffer object"));
  }

  return ecma_builtin_arraybuffer_slice (this_arg, argument_list_p, arguments_number);
} /* ecma_builtin_arraybuffer_prototype_object_slice */

/**
 * @}
 * @}
 * @}
 */

#endif /* JERRY_BUILTIN_TYPEDARRAY */
