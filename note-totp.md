
RFC 6238 - TOTP
---------------

    TOTP = HOTP(K, T)

where T: # of time steps b/w T0 and current time


    T = (Current Unix Time - T0) / X
   

RFC 4226 - HTOP
---------------

    HOTP(K, C) = Truncate(HMAC-SHA-1(K,C))


    HS = HMAC-SHA-1(K,C) :: byte string with length 20
    Sbits = DynamicTruncation(HS) :: 31-bit string
    Snum = StToNum(Sbits) :: uint32
    D = Snum mod 10^Digit



RFC 2104 - HMAC
---------------

    Inner pad: ipad = repeat 0x36, B times
    Outer pad: opad = repeat 0x5C, B times

    HMAC-H(K) = H(*K* XOR opad, H(*K* XOR ipad, text))

