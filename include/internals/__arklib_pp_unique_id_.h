#ifndef ARKLIB_INTERNALS_UNIQUE_ID_H_
#define ARKLIB_INTERNALS_UNIQUE_ID_H_

#define __UNIQUE_ID_(Base_, N_)	  ___UNIQUE_ID__ (Base_, N_)
#define ___UNIQUE_ID__(Base_, N_) Base_##N_

#endif /* ARKLIB_INTERNALS_UNIQUE_ID_H_ */
