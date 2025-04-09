#include "token_int.h"

void	announce(const char *cllr, char *raw, char *str, int msg)
{
	if (!cllr)
		return ;
	if (SAYGOT == msg)
	{
		if (str)
			dprint(_MOD_ ": %s: got _%s_, _%s_(cache)\n", cllr, raw, str);
		else
			dprint(_MOD_ ": %s: got _%s_\n", cllr, raw);
	}
	else if (SAYDONE == msg)
		dprint(_MOD_ ": %s: returning _%s_(cache)\n", cllr, str);
	else if (SAYCANCOMB == msg)
		dprint(_MOD_ ": %s: got comb'ble _%s_\n", cllr, raw);
	else if (SAYCANNOTCOMB == msg)
		dprint(_MOD_ ": %s: got uncomb'ble %s, add'g cached _%s_\n", cllr, raw,
			str);
	else if (SAYCOMBINE == msg)
		dprint(_MOD_ ": %s: combn'g _%s_ + _%s_(cache)\n", cllr, raw, str);
	else if (SAYTRIMMED == msg)
		dprint(_MOD_ ": %s: only trim frnt _%s_ + rr _%s_\n", cllr, raw, str);
	else if (SAYCACHEONLY == msg)
		dprint(_MOD_ ": %s: only caching _%s_, can't comb _%s_(cache)\n", cllr,
			raw, str);
	else
		dprint(_MOD_ ": %s: got NULL\n", cllr);
}
