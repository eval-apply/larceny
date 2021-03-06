// Ops for SFixnum

        PREDICATE_OVERRIDE_TRUE(isNumber, numberp)
        PREDICATE_OVERRIDE_TRUE(isFixnum, fixnump)
        PREDICATE_OVERRIDE_TRUE(isComplex, complexp)
        PREDICATE_OVERRIDE_TRUE(isReal, realp)
        PREDICATE_OVERRIDE_TRUE(isRational, rationalp)
        PREDICATE_OVERRIDE_TRUE(isInteger, integerp)
        PREDICATE_OVERRIDE_TRUE(isExact, exactp)
        PREDICATE_OVERRIDE_FALSE(isInexact, inexactp)

        OP1_OVERRIDE(integer2char) {
            return Factory.makeChar(this.value);
        }
        OP2_OVERRIDE_REVERSED(typetag_set, STagged) {
            arg1.tag = this.value;
            return Factory.Unspecified;
        }
        OP2_OVERRIDE_REVERSED(vector_like_ref, SVL) {
            try {
              return (arg1.elements) [this.value];
            }
            catch (Exception) {
              Exn.fault (Constants.EX_VLREF, null, arg1, this);
                return Factory.Impossible;
            }
        }
        OP3_OVERRIDE_REVERSED(vector_like_set, SVL) {
          try {
            (arg1.elements)[this.value] = arg3;
                return Factory.Unspecified;
            }
          catch (Exception) {
                Exn.fault(Constants.EX_VLSET, null, arg1, this, arg3);
                return Factory.Impossible;
            }
        }
        OP2_OVERRIDE(make_vector) {
            int length = this.value;
            if (0 <= length) {
                return Factory.makeVector(this.value, arg2);
            } else {
                Exn.fault(Constants.EX_MKVL, null, this, arg2);
                return Factory.Impossible;
            }
        }
        OP2_OVERRIDE_REVERSED(vector_ref, SVL) {
          try {
            return (arg1.elements)[this.value];
          }
          catch (Exception) {
                Exn.fault(Constants.EX_VECTOR_REF, null, arg1, this);
                return Factory.Impossible;
            }
        }
        OP2_OVERRIDE_REVERSED(vector_ref_trusted, SVL) {
          return (arg1.elements)[this.value];
        }
        OP3_OVERRIDE_REVERSED(vector_set, SVL) {
          try {
            (arg1.elements)[this.value] = arg3;
                return Factory.Unspecified;
          }
          catch (Exception) {
                Exn.fault(Constants.EX_VECTOR_SET, null, arg1, this, arg3);
                return Factory.Impossible;
            }
        }
        OP3_OVERRIDE_REVERSED(vector_set_trusted, SVL) {
          (arg1.elements)[this.value] = arg3;
                return Factory.Unspecified;
        }
        OP1_OVERRIDE(make_procedure) {
            int a = this.value - 2;
            if (0 <= a) {
                SObject[] env = new SObject[a];
                env[0] = Factory.False;
                for (int i = 1; i < a; ++i) {
                    env[i] = Factory.Null; // So says Petit Larceny
                }
                return new Procedure(CodeVector.NoCode,
                                     Factory.makeVector(0, Factory.False),
                                     env);
            } else {
                Exn.fault(Constants.EX_MKVL, null, this);
                return Factory.Impossible;
            }
        }
        OP2_OVERRIDE_REVERSED(procedure_ref, Procedure) {
            Procedure p = arg1;
            int b = this.value;
            if (b == 0) {
                // Code vector
                return p.getCode();
            } else if (b == 1) {
                // Constant vector
                return p.constantvector;
            } else if (b > 1) {
                b = b - 2;
                if (b < p.rib.Length) {
                    return p.rib[b];
                }
            }
            Exn.fault(Constants.EX_PROCEDURE_REF, null, arg1, this);
            return Factory.Impossible;
        }
        OP3_OVERRIDE_REVERSED(procedure_set, Procedure) {
            Procedure p = arg1;
            int b = this.value;
            if (b == 0) {
                // Code vector
                p.setCode(arg3);
                return Factory.Unspecified;
            } else if (b == 1) {
                // Constant vector
                if (arg3.isVector()) {
                    p.setConstants((SVL)arg3);
                    return Factory.Unspecified;
                } else {
                    Exn.fault(Constants.EX_PROCEDURE_SET, null, arg1, this, arg3);
                    return Factory.Impossible;
                }
            } else if (b > 1) {
                b = b - 2;
                if (b < p.rib.Length) {
                    p.rib[b] = arg3;
		    if (b == 0)
			p.parent = arg3 as Procedure;
                    return Factory.Unspecified;
                }
            }
            Exn.fault(Constants.EX_PROCEDURE_SET, null, arg1, this, arg3);
            return Factory.Impossible;
        }

        OP2_CHAIN(make_string)
        OP2_OVERRIDE_REVERSED(string_ref, SByteVL) {
          try {
            return Factory.makeChar ((arg1.elements)[this.value]);
          }
          catch (Exception){
            Exn.fault(Constants.EX_STRING_REF, null, arg1, this);
            return Factory.Impossible;
          }
        }
        OP2_OVERRIDE_REVERSED(string_ref_trusted, SByteVL) {
          return Factory.makeChar ((arg1.elements)[this.value]);
        }
        OP3_OVERRIDE_REVERSED(string_set, SByteVL) {
          try {
            (arg1.elements)[this.value] = (byte) ((SChar)arg3).val;
                    return Factory.Unspecified;
                }
          catch (Exception) {
            // FIXME: Should this instead throw a string-set exception?
            Exn.fault(Constants.EX_STRING_REF, null, arg1, this);
            return Factory.Impossible;
          }
        }
        OP3_OVERRIDE_REVERSED(string_set_trusted, SByteVL) {
          (arg1.elements)[this.value] = unchecked ((byte) ((SChar)arg3).val);
            return Factory.Unspecified;
        }

        OP2_CHAIN(make_ustring)
        OP2_OVERRIDE_REVERSED(ustring_ref, SByteVL) {
          try {
            return Factory.makeChar
              (  (arg1.elements)[(this.value << 2)    ] << 24
               | (arg1.elements)[(this.value << 2) + 1] << 16
               | (arg1.elements)[(this.value << 2) + 2] << 8
               | (arg1.elements)[(this.value << 2) + 3]);
          }
          catch (Exception) {
            Exn.fault(Constants.EX_STRING_REF, null, arg1, this);
            return Factory.Impossible;
          }
        }
        OP2_OVERRIDE_REVERSED(ustring_ref_trusted, SByteVL) {
          return Factory.makeChar
            (  (arg1.elements)[(this.value << 2)    ] << 24
             | (arg1.elements)[(this.value << 2) + 1] << 16
             | (arg1.elements)[(this.value << 2) + 2] << 8
             | (arg1.elements)[(this.value << 2) + 3]);
        }

        OP3_OVERRIDE_REVERSED(ustring_set, SByteVL) {
          try {
            int val = ((SChar)arg3).val;

            arg1.elements[(this.value << 2)]     = (byte)(val >> 24 & 0xFF);
            arg1.elements[(this.value << 2) + 1] = (byte)(val >> 16 & 0xFF);
            arg1.elements[(this.value << 2) + 2] = (byte)(val >> 8 & 0xFF);
            arg1.elements[(this.value << 2) + 3] = (byte)(val & 0xFF);

            return Factory.Unspecified;
          }
          catch (Exception) {
            // FIXME: Should this instead throw a ustring-set exception?
            Exn.fault(Constants.EX_STRING_REF, null, arg1, this);
            return Factory.Impossible;
          }
        }
        OP3_OVERRIDE_REVERSED(ustring_set_trusted, SByteVL) {
          int val = ((SChar)arg3).val;

          arg1.elements[(this.value << 2)]     = (byte)(val >> 24 & 0xFF);
          arg1.elements[(this.value << 2) + 1] = (byte)(val >> 16 & 0xFF);
          arg1.elements[(this.value << 2) + 2] = (byte)(val >> 8 & 0xFF);
          arg1.elements[(this.value << 2) + 3] = (byte)(val & 0xFF);

          return Factory.Unspecified;
        }

        OP1_OVERRIDE(make_bytevector) {
            return Factory.makeByteVector(this.value, (byte)0);
        }
        OP2_OVERRIDE_REVERSED(bytevector_ref, SByteVL) {
          try {
            return makeFixnum ((arg1.elements)[this.value]);
            }
          catch (Exception) {
                Exn.fault(Constants.EX_BYTEVECTOR_REF, null, arg1, this);
                return Factory.Impossible;
            }
        }
        OP3_OVERRIDE_REVERSED(bytevector_set, SByteVL) {
          try {
            (arg1.elements) [this.value] = (byte) ((SFixnum)arg3).value;
                    return Factory.Unspecified;
                }
          catch (Exception) {
            Exn.fault(Constants.EX_BYTEVECTOR_SET, null, arg1, this, arg3);
            return Factory.Impossible;
        }
        }
        OP2_OVERRIDE_REVERSED(bytevector_fill, SByteVL) {
            byte[] bytes = arg1.elements;
            byte fill = (byte) this.value;
            for (int i = 0; i < bytes.Length; ++i) {
               bytes[i] = fill;
            }
            return Factory.Unspecified;
        }
        
        OP2_OVERRIDE_REVERSED(bytevector_like_ref, SByteVL) {
          try {
            return makeFixnum ((arg1.elements)[this.value]);
          }
          catch (Exception) {
                Exn.fault(Constants.EX_BVLREF, null, arg1, this);
                return Factory.Impossible;
            }
        }
        OP3_OVERRIDE_REVERSED(bytevector_like_set, SByteVL) {
          try {
            (arg1.elements)[this.value] = (byte)((SFixnum)arg3).value;
                    return Factory.Unspecified;
                }
          catch (Exception) {
            Exn.fault(Constants.EX_BVLREF, null, arg1, this);
            return Factory.Impossible;
          }
        }

        OP1_OVERRIDE(fxzerop) {
            return (this.value == 0) ? Factory.True : Factory.False;
        }
        OP1_OVERRIDE(fxpositivep) {
            return (this.value > 0) ? Factory.True : Factory.False;
        }
        OP1_OVERRIDE(fxnegativep) {
            return (this.value < 0) ? Factory.True : Factory.False;
        }
        OP1_OVERRIDE(fxnegative) {
            int a = - this.value;
            if (!inFixnumRange(a)) {
                Exn.fault(Constants.EX_FXNEG, "result not a fixnum", this);
                return Factory.Impossible;
            }
            return makeFixnum (a);
        }
        OP2_CHAIN(fxplus)
        OP2_CHAIN(fxminus)
        OP2_CHAIN(fxmul)
        OP2_CHAIN(fxless)
        OP2_CHAIN(fxless_equal)
        OP2_CHAIN(fxgreater)
        OP2_CHAIN(fxgreater_equal)
        OP2_CHAIN(fxequal)

        OP2_OVERRIDE_REVERSED(fxplus, SFixnum) {
            int a = arg1.value, b = this.value; 
            int r = a + b; 
            if (!inFixnumRange(r)) {
                Exn.fault(Constants.EX_FXADD, null, arg1, this);
                return Factory.Impossible; 
            }
            return makeFixnum (r);
        }
        OP2_OVERRIDE_REVERSED(fxminus, SFixnum) {
            int a = arg1.value, b = this.value; 
            int r = a - b; 
            if (!inFixnumRange(r)) {
                Exn.fault(Constants.EX_FXSUB, null, arg1, this); 
                return Factory.Impossible; 
            }
            return makeFixnum (r);
        }
        OP2_OVERRIDE_REVERSED(fxmul, SFixnum) {
            int a = arg1.value, b = this.value;
            long r = a * b;
            if (!inFixnumRange(r)) {
                Exn.fault(Constants.EX_FXMUL, null, arg1, this);
                return Factory.Impossible;
            }
            return makeFixnum ((int)r);
        }
        OP2_OVERRIDE_REVERSED(fxless, SFixnum) {
            return (arg1.value < this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(fxless_equal, SFixnum) {
            return (arg1.value <= this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(fxgreater, SFixnum) {
            return (arg1.value > this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(fxgreater_equal, SFixnum) {
            return (arg1.value >= this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(fxequal, SFixnum) {
            return (arg1.value == this.value) ? Factory.True : Factory.False;
        }

        OP1_OVERRIDE(fxlognot) {
            return Factory.makeNumber (~this.value);
        }
        OP2_CHAIN(fxlogand)
        OP2_CHAIN(fxlogior)
        OP2_CHAIN(fxlogxor)
        OP2_CHAIN(fxlsh)
        OP2_CHAIN(fxrsh)
        OP2_CHAIN(fxrsha)
        OP2_CHAIN(fxrshl)

        OP2_OVERRIDE_REVERSED(fxlogand, SFixnum) {
            return makeFixnum (arg1.value & this.value);
        }
        OP2_OVERRIDE_REVERSED(fxlogior, SFixnum) {
            return makeFixnum (arg1.value | this.value);
        }
        OP2_OVERRIDE_REVERSED(fxlogxor, SFixnum) {
            return makeFixnum (arg1.value ^ this.value);
        }
        OP2_OVERRIDE_REVERSED(fxlsh, SFixnum) {
            int r = arg1.value << this.value;
            r = (r << 2) >> 2; // mask out top bits (w/ sign extend)
            return makeFixnum (r);
        }
        OP2_OVERRIDE_REVERSED(fxrsh, SFixnum) {
            int r = arg1.value >> this.value;
            // r = (r << 2) >> 2;
            return makeFixnum (r);
        }
        OP2_OVERRIDE_REVERSED(fxrsha, SFixnum) {
            int r = arg1.value >> this.value;
            // r = (r << 2) >> 2;
            return makeFixnum (r);
        }
        OP2_OVERRIDE_REVERSED(fxrshl, SFixnum) {
            uint a_preshift = unchecked((uint)arg1.value);
            uint a = a_preshift << 2;
            int b = this.value;
            int r = (int)(a >> b) >> 2;
            return Factory.makeNumber ((int)r);
        }

        OP1_OVERRIDE(real_part) {
            return this;
        }
        OP1_OVERRIDE(imag_part) {
            return makeFixnum (0);
        }

        OP2_CHAIN(plus_idx_idx)
        OP2_CHAIN(plus_fix_fix)
        OP2_CHAIN(minus_idx_idx)
        OP2_CHAIN(minus_fix_fix)
        OP2_CHAIN(equal_fix_fix)
        OP2_CHAIN(less_fix_fix)
        OP2_CHAIN(lessequal_fix_fix)
        OP2_CHAIN(greater_fix_fix)
        OP2_CHAIN(greaterequal_fix_fix)

        OP2_OVERRIDE_REVERSED(plus_idx_idx, SFixnum) {
            return Factory.makeNumber (arg1.value + this.value);
        }
        OP2_OVERRIDE_REVERSED(plus_fix_fix, SFixnum) {
            return Factory.makeNumber (arg1.value + this.value);
        }
        OP2_OVERRIDE_REVERSED(minus_idx_idx, SFixnum) {
            return Factory.makeNumber (arg1.value - this.value);
        }
        OP2_OVERRIDE_REVERSED(minus_fix_fix, SFixnum) {
            return Factory.makeNumber (arg1.value - this.value);
        }
        OP2_OVERRIDE_REVERSED(equal_fix_fix, SFixnum) {
            return (arg1.value == this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(less_fix_fix, SFixnum) {
            return (arg1.value < this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(lessequal_fix_fix, SFixnum) {
            return (arg1.value <= this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(greater_fix_fix, SFixnum) {
            return (arg1.value > this.value) ? Factory.True : Factory.False;
        }
        OP2_OVERRIDE_REVERSED(greaterequal_fix_fix, SFixnum) {
            return (arg1.value >= this.value) ? Factory.True : Factory.False;
        }

        // Special Operations

        SPECIALOP1_OVERRIDE(enable_interrupts) {
            int time = this.value;
            if (time > 0) {
                Reg.interruptsEnabled = true;
                Reg.timer = time;
            } else {
                Exn.fault(Constants.EX_EINTR, null, this);
            }
            Reg.Result = Factory.Unspecified;
            Exn.checkSignals();
        }
        SPECIALOP1_OVERRIDE(syscall) {
            // subtract one 'cuz the first arg is just the value
            // to which we want to dispatch.
            int num_args = this.value - 1;
            Sys num_syscall = (Sys) ((SFixnum)Reg.Register1).intValue();
            Syscall.dispatch(num_args, num_syscall);
        }
        SPECIALOP1_OVERRIDE(zerop) {
          Reg.Result = (this.value == 0) ? Factory.True : Factory.False;
        }

        SPECIALOP2_CHAIN_FIXNUM(eqvp_not_eq)
        SPECIALOP2_CHAIN_FIXNUM(numeric_equals)
        SPECIALOP2_CHAIN_FIXNUM(less_than)
        SPECIALOP2_CHAIN_FIXNUM(less_or_equal)
        SPECIALOP2_CHAIN_FIXNUM(greater_than)
        SPECIALOP2_CHAIN_FIXNUM(greater_or_equal)

        SPECIALOP2_CHAIN_FIXNUM(plus)
        SPECIALOP2_CHAIN_FIXNUM(minus)
        SPECIALOP2_CHAIN_FIXNUM(multiply)
        SPECIALOP2_CHAIN_FIXNUM(divide)
        SPECIALOP2_CHAIN_FIXNUM(quotient)
        SPECIALOP2_CHAIN_FIXNUM(remainder)

        SPECIALOP2_OVERRIDE_REVERSED_CASE(eqvp_not_eq, fixnum, SFixnum) {
            Reg.Result = (arg1.value == this.value) ? Factory.True : Factory.False;
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(numeric_equals, fixnum, SFixnum) {
            Reg.Result = (arg1.value == this.value) ? Factory.True : Factory.False;
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(less_than, fixnum, SFixnum) {
            Reg.Result = (arg1.value < this.value) ? Factory.True : Factory.False;
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(less_or_equal, fixnum, SFixnum) {
            Reg.Result = (arg1.value <= this.value) ? Factory.True : Factory.False;
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(greater_than, fixnum, SFixnum) {
            Reg.Result = (arg1.value > this.value) ? Factory.True : Factory.False;
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(greater_or_equal, fixnum, SFixnum) {
            Reg.Result = (arg1.value >= this.value) ? Factory.True : Factory.False;
        }

        SPECIALOP2_OVERRIDE_REVERSED_CASE(plus, fixnum, SFixnum) {
            Reg.Result = Factory.makeNumber (arg1.value + this.value);
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(minus, fixnum, SFixnum) {
            Reg.Result = Factory.makeNumber (arg1.value - this.value);
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(multiply, fixnum, SFixnum) {
            long a = arg1.value;
            long b = this.value;
            Reg.Result = Factory.makeNumber (a * b);
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(divide, fixnum, SFixnum) {
            int a = arg1.value, b = this.value;
            if (b == 0) {
                Exn.fault(Constants.EX_DIV, null, arg1, this);
            }
            if (a % b == 0) {
                Reg.Result = Factory.makeNumber (a / b);
            } else {
                Call.callMillicodeSupport2(Constants.MS_FIXNUM2RATNUM_DIV, arg1, this);
            }
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(quotient, fixnum, SFixnum) {
            int a = arg1.value, b = this.value;
            if (b == 0) {
                Exn.fault(Constants.EX_QUOTIENT, null, arg1, this);
            }
            Reg.Result = Factory.makeNumber (a / b);
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(quotient, bignum, SByteVL) {
            // Must handle 1-word bignums too.
            // Exn.debug.WriteLine("trying bignum/fixnum case: {0}; {1}", arg1, this);
            int bv = this.value;
            if (bv == 0) {
                Exn.fault(Constants.EX_QUOTIENT, null, arg1, this);
                return;
            } else if (bv > 0 &&
                       Number.getBignumLength(arg1) == 1 && 
                       Number.getBignumSign(arg1)) {
                uint av = arg1.getUInt32(1);
                uint result = av / (uint)bv;
                // Exn.debug.WriteLine("  / {0}; {1}", av, bv);
                Reg.Result = Factory.makeNumber(result);
                return;
            }
            base.op_reversed_generic_quotient(arg1);
        }

        SPECIALOP2_OVERRIDE_REVERSED_CASE(remainder, fixnum, SFixnum) {
            int a = arg1.value, b = this.value;
            if (b == 0) {
                Exn.fault(Constants.EX_REMAINDER, null, arg1, this);
            }
            Reg.Result = Factory.makeNumber (a % b);
        }
        SPECIALOP2_OVERRIDE_REVERSED_CASE(remainder, bignum, SByteVL) {
            // Must handle 1-word bignums too.
            int bv = this.value;
            if (bv > 0 &&
                Number.getBignumLength(arg1) == 1 && 
                Number.getBignumSign(arg1)) {
                uint av = arg1.getUInt32(1);
                uint result = av % (uint)bv;
                Reg.Result = Factory.makeNumber(result);
                return;
            }
            base.op_reversed_generic_remainder(arg1);
        }

        SPECIALOP1_OVERRIDE(truncate) {
            Reg.Result = this;
        }
        SPECIALOP1_OVERRIDE(round) {
            Reg.Result = this;
        }
        SPECIALOP1_OVERRIDE(negative) {
            Reg.Result = Factory.makeNumber (-this.value);
        }
        SPECIALOP1_OVERRIDE(exact2inexact) {
            Reg.Result = Factory.makeFlonum((double)this.value);
        }
        SPECIALOP1_OVERRIDE(inexact2exact) {
            Reg.Result = this;
        }

	public override SObject op_eqp_int32(int n) { return (this.value == n) ? Factory.True : Factory.False; }
	public override SObject op_plus_idx_idx_int32(int n) { return Factory.makeNumber(this.value + n); }
	public override SObject op_minus_idx_idx_int32(int n) { return Factory.makeNumber(this.value - n); }
	public override void    op_numeric_equals_int32(int n) { Reg.Result = (this.value == n) ? Factory.True : Factory.False; }
	public override void op_plus_int32(int n) { Reg.Result = Factory.makeNumber( this.value + n ); }
	public override void op_minus_int32(int n) { Reg.Result = Factory.makeNumber( this.value - n); }
	public override SObject op_fxless_int32(int n) { return (this.value < n) ? Factory.True : Factory.False; }
	public override SObject op_greaterequal_fix_fix_int32(int n) { return (this.value >= n) ? Factory.True : Factory.False; }
	public override SObject op_less_fix_fix_int32(int n) { return (this.value < n) ? Factory.True : Factory.False; }

 	public override bool isEqpInt32(int n) { return this.value == n; }
 	public override bool isFxLessInt32(int n) { return this.value < n; }
 	public override bool isNumericEqualFixFixInt32(int n) { return this.value == n; }
 	public override bool isLessFixFixInt32(int n) { return this.value < n; }
