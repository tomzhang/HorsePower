## Blackscholes

A benchmark from the PARSEC benchmark suite.


```no-highlight
module default{
    import Builtin.*;
    def CNDF(OutputX:f64, InputX:f64, invs2xPI:f64){
        sign:f64       = @lt(InputX, 0:f64);
        xInput:f64     = @abs(InputX);
        t0:f64         = @mul(InputX,InputX);
        t1:f64         = @mul(0.5:f64, t0);
        t2:f64         = @neg(t1);
        expValues:f64  = @exp(t2);
        xNPrimeofX:f64 = @mul(invs2xPI, expValues);
        t3:f64         = @mul(0.2316419:f64, xInput);
        t4:f64         = @plus(1:f64, t3);
        xK2:f64        = @plus(1:f64, t4);
        xK2_2:f64      = @mul(xK2, xK2);
        xK2_3:f64      = @mul(xK2, xK2_2);
        xK2_4:f64      = @mul(xK2, xK2_3);
        xK2_5:f64      = @mul(xK2, xK2_4);
        xLocal_1:f64   = @mul(xK2, 0.319381530:f64);
        t5:f64         = @mul(xK2_2, -0.356563782);
        t6:f64         = @mul(xK2_3, 1.781477937);
        t7:f64         = @mul(xK2_4, -1.821255978);
        t8:f64         = @mul(xK2_5, 1.330274429);
        t9:f64         = @plus(t5,t6);
        t10:f64        = @plus(t9,t7);
        xLocal_2:f64   = @plus(t10, t8);
        t11:f64        = @plus(xLocal_2, xLocal_1);
        t12:f64        = @mul(xNPrimeofX, t11);
        xLocal:f64     = @minus(1.0:f64, t12);
        t13:f64        = @not(sign);
        t14:f64        = @mul(t13, xLocal);
        t15:f64        = @minus(1.0:f64, xLocal);
        t16:f64        = @mul(sign, t15);
        OutputX:f64    = @plus(t16,t14);
        return OutputX;
    }
    def BlkSchls(sptprice:f64, strike:f64, rate:f64, divq:f64, volatility:f64, time:f64, optiontype:char, divs:f64, dfrefval:f64) : f64 {
        xSqrtTime:f64     = @power(time:f64,  0.5);
        t0:f64            = @div(sptprice, strike);
        xLogTerm:f64      = @log(xLogTerm, t0);
        xRiskFreeRate:f64 = rate;
        xDen:f64          = @mul(volatility,xSqrtTime);
        t1:f64            = @mul(volatility, 0.5:f64);
        xPowerTerm:f64    = @mul*volatility, t1);
        t2:f64            = @plus(xRiskFreeRate, xPowerTerm);
        t3:f64            = @mul(time, t2);
        t4:f64            = @plus(xLogTerm, t3);
        xD1:f64           = @div(t4, xDen);
        d1:f64            = xD1;
        xD2:f64           = @minus(xD1, xDen);
        d2:f64            = xD2;
        invs2xPI:f64      = 0.39894228040143270286:f64;
        NofXd1:f64        = @CNDF(d1, invs2xPI); 
        NofXd2:f64        = @CNDF(d2, invs2xPI); 
        t5:f64            = @mul(rate, time);
        t6:f64            = @neg(t5);
        t7:f64            = @exp(t6);
        FutureValueX:f64  = @mul(strike,t7);
        t8:f64            = @minus(1.0:f64, NofXd1);
        t9:f64            = @mul(sptprice, t8);
        t10:f64           = @minus(1.0:f64, NofXd2);
        t11:f64           = @mul(FutureValueX, t10);
        t12:f64           = @minus(t11,t9);
        OptionPrice:f64   = @mul(otype, t12);
        t13:f64           = @mul(FutureValueX, NofXd2);
        t14:f64           = @mul(sptprice, NofXd1);
        t15:f64           = @minus(t14,t13);
        t16:f64           = @not(otype);
        t17:f64           = @mul(t16,t15);
        OptionPrice:f64   = @plus(OptionPrice, t17);
        return OptionPrice;
    }

    def bs0_base() : table {
        // Load a table "blackscholess_table" into "bs"
        bs:table        = @load_table(`blackscholes_table);

        // Load all columns by its column names and save to in sptprice and etc.
        sptprice:f64    = check_cast(@column_value(bs, `sptprice)  , f64);
        strike:f64      = check_cast(@column_value(bs, `strike)    , f64);
        rate:f64        = check_cast(@column_value(bs, `rate)      , f64);
        divq:f64        = check_cast(@column_value(bs, `divq)      , f64);
        volatility:f64  = check_cast(@column_value(bs, `volatility), f64);
        time:f64        = check_cast(@column_value(bs, `time)      , f64);
        optiontype:char = check_cast(@column_value(bs, `optiontype), char);
        divs:f64        = check_cast(@column_value(bs, `divs)      , f64);
        dgrefval:f64    = check_cast(@column_value(bs, `dgrefval)  , f64);

        // Pass all columns to "@BlkSchls" and save its output to "optionprice"
        optionprice:f64 = @BlkSchls(sptprice,strike,rate,divq,volatility,time,optiontype,divs,dgrefval);

        // Construct a new table (with optionprice) and return it
        columnname:sym      = (`optionprice,`optiontype,`sptprice):sym;
        columnvalue:list<?> = @list(optiontype, sptprice, optionprice);
        result:table        = @table(columnname, columnvalue);
        return result;
    }

    def bs1_high() : table {
        // Load a table "blackscholess_table" into "bs"
        bs:table        = @load_table(`blackscholes_table);

        // Load all columns by its column names and save to in sptprice and etc.
        sptprice:f64    = check_cast(@column_value(bs, `sptprice)  , f64);
        strike:f64      = check_cast(@column_value(bs, `strike)    , f64);
        rate:f64        = check_cast(@column_value(bs, `rate)      , f64);
        divq:f64        = check_cast(@column_value(bs, `divq)      , f64);
        volatility:f64  = check_cast(@column_value(bs, `volatility), f64);
        time:f64        = check_cast(@column_value(bs, `time)      , f64);
        optiontype:char = check_cast(@column_value(bs, `optiontype), char);
        divs:f64        = check_cast(@column_value(bs, `divs)      , f64);
        dgrefval:f64    = check_cast(@column_value(bs, `dgrefval)  , f64);

        // Apply filters first
        w0:bool         = @lt(sptprice, 50:i64);
        w1:bool         = @gt(sptprice, 100:i64);
        w2:bool         = @and(w0, w1);

        m0:f64          = @compress(w3, sptprice);
        m1:f64          = @compress(w3, strike);
        m2:f64          = @compress(w3, rate);
        m3:f64          = @compress(w3, divq);
        m4:f64          = @compress(w3, volatility);
        m5:f64          = @compress(w3, time);
        m6:char         = @compress(w3, optiontype);
        m7:f64          = @compress(w3, divs);
        m8:f64          = @compress(w3, dgrefval);

        // Pass all "selected" columns to "@BlkSchls" and save its output to "optionprice"
        optionprice:f64 = @BlkSchls(m0,m1,m2,m3,m4,m5,m6,m7,m8);

        // Construct a new table (with optionprice) and return it
        columnname:sym      = (`optionprice,`optiontype,`sptprice):sym;
        columnvalue:list<?> = @list(optiontype, sptprice, optionprice);
        result:table        = @table(columnname, columnvalue);
        return result;
    }
    def main() : table {
        return bs0_base();  //Or  return bs1_high();
    }
}
```


