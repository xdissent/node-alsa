#ifndef MACROS_H
#define MACROS_H

#define NODE_SET_GETTER(target, name, function)                                \
  (target)->InstanceTemplate()                                                 \
    ->SetAccessor(String::NewSymbol(name), (function));

#define REQUIRE_ARGUMENTS(n)                                                   \
  if (args.Length() < (n)) {                                                   \
    return ThrowException(                                                     \
      Exception::TypeError(String::New("Expected " #n "arguments"))            \
    );                                                                         \
  }

#define COND_ERR_CALL_VOID(condition, callback, message, context)              \
  if (condition) {                                                             \
    if ((callback).IsEmpty() || !(callback)->IsFunction()) {                   \
      ThrowException(Exception::TypeError(String::New(message)));              \
      return;                                                                  \
    }                                                                          \
    Local<Value> exception = Exception::Error(String::New(message));           \
    Local<Value> argv[1] = { Local<Value>::New(exception) };                   \
    TRY_CATCH_CALL((context), (callback), 1, argv);                            \
    return;                                                                    \
  }

#define COND_ERR_CALL(condition, callback, message)                            \
  if (condition) {                                                             \
    if ((callback).IsEmpty() || !(callback)->IsFunction())                     \
      return ThrowException(Exception::TypeError(String::New(message)));       \
    Local<Value> exception = Exception::Error(String::New(message));           \
    Local<Value> argv[1] = { Local<Value>::New(exception) };                   \
    TRY_CATCH_CALL(args.Holder(), (callback), 1, argv);                        \
    return scope.Close(Undefined());                                           \
  }

#define OPTIONAL_ARGUMENT_FUNCTION(i, var)                                     \
  Local<Function> var;                                                         \
  if (args.Length() > i && !args[i]->IsUndefined()) {                          \
    if (!args[i]->IsFunction()) {                                              \
      return ThrowException(Exception::TypeError(                              \
        String::New("Argument " #i " must be a function"))                     \
      );                                                                       \
    }                                                                          \
    var = Local<Function>::Cast(args[i]);                                      \
  }

#define REQUIRE_ARGUMENT_FUNCTION(i, var)                                      \
  if (args.Length() <= (i) || !args[i]->IsFunction()) {                        \
    return ThrowException(Exception::TypeError(                                \
      String::New("Argument " #i " must be a function"))                       \
    );                                                                         \
  }                                                                            \
  Local<Function> var = Local<Function>::Cast(args[i]);

#define TRY_CATCH_CALL(context, callback, argc, argv)                          \
{   TryCatch try_catch;                                                        \
    (callback)->Call((context), (argc), (argv));                               \
    if (try_catch.HasCaught()) {                                               \
        FatalException(try_catch);                                             \
    }                                                                          }

#endif