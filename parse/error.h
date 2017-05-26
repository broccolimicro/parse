#pragma once

#define FAIL(msg) result.fail(__FILE__, __LINE__, msg)
#define ERROR(msg) result.error(__FILE__, __LINE__, msg)
#define WARN(msg) result.warn(__FILE__, __LINE__, msg)
#define NOTE(msg) result.note(__FILE__, __LINE__, msg)
#define ELAB(msg) result.elab(__FILE__, __LINE__, msg)

