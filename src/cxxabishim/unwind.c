struct _Unwind_Exception;
extern int _Unwind_RaiseException(struct _Unwind_Exception *exception_object);
int _Unwind_SjLj_RaiseException(struct _Unwind_Exception *exception_object) {
	return _Unwind_RaiseException(exception_object);
}

