
cctools
=======
 - Its source code can be obtained from [Apple Open Source](http://opensource.apple.com/)
 - This is the most important tool if you want a compiler which targets iOS.
 - as, ar, ld, ranlib, lipo, etc.
 - The origianl project from Apple does not compile on Linux
   * (MAYBE WRONG) Requires headers from XNU, another open source project from Apple.
   * Requires headers from MacOSX SDK.
     (Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/usr/include/)
   * It's a bit trickier with newer XNU headers
   * Makefiles are dirty. Need some work.
