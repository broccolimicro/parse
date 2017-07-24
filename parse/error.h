#pragma once

#define FAIL(ctx, loc, msg) (ctx).fail(__FILE__, __LINE__, (msg), (loc))
#define ERROR(ctx, loc, msg) (ctx).error(__FILE__, __LINE__, (msg), (loc))
#define WARN(ctx, loc, msg) (ctx).warn(__FILE__, __LINE__, (msg), (loc))
#define NOTE(ctx, loc, msg) (ctx).note(__FILE__, __LINE__, (msg), (loc))
#define ELAB(ctx, msg) (ctx).elab(__FILE__, __LINE__, (msg))


