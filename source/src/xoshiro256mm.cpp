/**
 * @file random_xor_combine_core.h
 * @brief 伪随机数生成器  - xoshiro算法核心
 * @see http://xoshiro.di.unimi.it
 * @note 2018年的新型的全功能型伪随机数算法，也是lua 5.4内建的伪随机数算法
 *
 * Licensed under the MIT licenses.
 * @version 1.0
 * @author OWenT
 * @date 2018年09月30日
 *
 *  7. 更好的随机数生成方法
尽管 LCG 计算简单，但如果需要更高质量的随机数，推荐：

Xorshift(LFSR)：更快，适用于高效随机数生成
PCG（Permuted Congruential Generator）：高质量 + 低计算量
Crypto-safe PRNGs（如 ChaCha20）：适用于密码学


 * 1. 上半部是xoshiro / xoroshiro算法实现1
 2. 中部是随机数功能封装，用于提供多引擎下统1一的随机数接口。剥离里项目中的梅森旋转和TAUS88引擎。
 3. 下半部是简化的单元测试，单元测试数据是采集自原作者的纯C代码的运行结果。
 * @history
 *
 */


 #ifndef UTIL_RANDOM_XOSHIRO_CORE_H
 #define UTIL_RANDOM_XOSHIRO_CORE_H
 
 // #pragma once
 
 #include <stdint.h>
 #include <cstddef>
 
 namespace util {
     namespace random {
         namespace core {
             /**
              * @breif just for
              *          xoshiro256**
              *          xoshiro256+
              *          xoroshiro128**
              *          xoroshiro128+
              * @note not support for xoroshiro64** 、xoroshiro64*、xoroshiro512** 、xoroshiro512*、xoroshiro1024** 、xoroshiro1024*
              */
             template <class UIntType, bool is_plus, int iidx, int n1, int n2>
             class xoshinro_engine {
             public:
                 typedef UIntType    result_type;
                 typedef result_type seed_type[4];
 
             private:
                 /// \endcond
                 seed_type xoshinro_seed_;
 
                 static inline result_type rotl(const result_type x, int k) {
                     return (x << k) | (x >> ((sizeof(result_type) * 8) - static_cast<result_type>(k)));
                 }
 
                 template <class, bool>
                 struct next_init;
 
                 template <class T>
                 struct next_init<T, true> {
                     static inline result_type call(seed_type &s) { return s[0] + s[3]; }
                 };
 
                 template <class T>
                 struct next_init<T, false> {
                     static inline result_type call(seed_type &s) { return rotl(s[iidx] * 5, 7) * 9; }
                 };
 
             protected:
                 result_type next() {
                     const result_type ret = next_init<UIntType, is_plus>::call(xoshinro_seed_);
                     const result_type t   = xoshinro_seed_[1] << n1;
 
                     xoshinro_seed_[2] ^= xoshinro_seed_[0];
                     xoshinro_seed_[3] ^= xoshinro_seed_[1];
                     xoshinro_seed_[1] ^= xoshinro_seed_[2];
                     xoshinro_seed_[0] ^= xoshinro_seed_[3];
 
                     xoshinro_seed_[2] ^= t;
                     xoshinro_seed_[3] = rotl(xoshinro_seed_[3], n2);
 
                     return ret;
                 }
 
                 void jump(const seed_type &JUMP) {
                     result_type s0 = 0;
                     result_type s1 = 0;
                     result_type s2 = 0;
                     result_type s3 = 0;
                     for (size_t i = 0; i < sizeof(JUMP) / sizeof(JUMP[0]); i++) {
                         for (size_t b = 0; b < sizeof(result_type) * 8; b++) {
                             if (JUMP[i] & result_type(1) << b) {
                                 s0 ^= xoshinro_seed_[0];
                                 s1 ^= xoshinro_seed_[1];
                                 s2 ^= xoshinro_seed_[2];
                                 s3 ^= xoshinro_seed_[3];
                             }
                             next();
                         }
                     }
 
                     xoshinro_seed_[0] = s0;
                     xoshinro_seed_[1] = s1;
                     xoshinro_seed_[2] = s2;
                     xoshinro_seed_[3] = s3;
                 }
 
             public:
                 xoshinro_engine() {
                     xoshinro_seed_[0] = 0;
                     xoshinro_seed_[1] = 0;
                     xoshinro_seed_[2] = 0;
                     xoshinro_seed_[3] = 0;
                 }
                 xoshinro_engine(result_type s) {
                     xoshinro_seed_[0] = 0;
                     xoshinro_seed_[1] = 0;
                     xoshinro_seed_[2] = 0;
                     xoshinro_seed_[3] = 0;
                     init_seed(s);
                 }
 
                 void init_seed(result_type s) {
                     xoshinro_seed_[0] = s;
                     xoshinro_seed_[1] = 0xff;
                     xoshinro_seed_[2] = 0;
                     xoshinro_seed_[3] = 0;
 
                     // just like in lua 5.4
                     for (int i = 0; i < 16; ++i) {
                         next();
                     }
                 }
 
                 template <class It>
                 void init_seed(It &first, It last) {
                     It begin = first;
                     for (int i = 0; i < 4; ++i) {
                         if (begin != last) {
                             xoshinro_seed_[i] = *begin;
                             ++begin;
                         } else {
                             xoshinro_seed_[i] = 0;
                         }
                     }
 
                     // just like in lua 5.4
                     for (int i = 0; i < 16; ++i) {
                         next();
                     }
                 }
 
                 result_type random() { return next(); }
 
                 result_type operator()() { return random(); }
 
                 inline const seed_type &get_seed() const { return xoshinro_seed_; }
             };
 
             template <bool is_plus>
             class xoshinro_engine_128 : public xoshinro_engine<uint32_t, is_plus, 0, 9, 11> {
             public:
                 typedef xoshinro_engine<uint32_t, is_plus, 0, 9, 11> base_type;
                 typedef typename base_type::result_type              result_type;
                 typedef typename base_type::seed_type                seed_type;
 
             public:
                 xoshinro_engine_128() {}
                 xoshinro_engine_128(result_type s) : base_type(s) {}
 
                 using base_type::jump;
 
                 /**
                  * @brief just like call next() for 2^64 times
                  */
                 void jump() {
                     static const result_type jump_params[4] = {0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b};
                     jump(jump_params);
                 }
             };
 
             template <bool is_plus>
             class xoshinro_engine_256 : public xoshinro_engine<uint64_t, is_plus, 1, 17, 45> {
             public:
                 typedef xoshinro_engine<uint64_t, is_plus, 1, 17, 45> base_type;
                 typedef typename base_type::result_type               result_type;
                 typedef typename base_type::seed_type                 seed_type;
 
             public:
                 xoshinro_engine_256() {}
                 xoshinro_engine_256(result_type s) : base_type(s) {}
 
                 using base_type::jump;
 
                 /**
                  * @brief just like call next() for 2^128 times
                  */
                 void jump() {
                     static const result_type jump_params[4] = {0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa,
                                                                0x39abdc4529b1661c};
                     jump(jump_params);
                 }
 
                 /**
                  * @brief just like call next() for 2^192 times
                  */
                 void long_jump() {
                     static const result_type jump_params[4] = {0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241,
                                                                0x39109bb02acbe635};
                     jump(jump_params);
                 }
             };
         } // namespace core
     }     // namespace random
 } // namespace util
 
 
 #endif /* UTIL_RANDOM_XOSHIRO_CORE_H */
 
 // ============================ 随机数引擎封装 ================================
 /**
  * @file random_generator.h
  * @brief 伪随机数生成器
  *
  * Licensed under the MIT licenses.
  * @version 1.0
  * @author OWenT
  * @date 2018年09月30日
  *
  * @history
  *
  */
 
 #ifndef UTIL_RANDOM_GENERATOR_H
 #define UTIL_RANDOM_GENERATOR_H
 
 // #pragma once
 
 #include <limits>
 #include <numeric>
 #include <ctime>
 
 // #include "random_mt_core.h"
 // #include "random_xor_combine_core.h"
 // #include "random_xoshiro_core.h"
 
 namespace util {
     namespace random {
         /**
          * 随机数包装类，用以提供高级功能
          */
         template <typename CoreType>
         class random_manager_wrapper {
         public:
             typedef CoreType                        core_type;
             typedef typename core_type::result_type result_type;
 
         private:
             core_type core_;
 
         public:
             random_manager_wrapper() {}
             random_manager_wrapper(result_type rd_seed) : core_(rd_seed) {}
 
             inline core_type &      get_core() { return core_; }
             inline const core_type &get_core() const { return core_; }
 
             /**
              * 初始化随机数种子
              * @param [in] rd_seed 随机数种子
              */
             void init_seed(result_type rd_seed) { core_.init_seed(rd_seed); }
 
             /**
              * 初始化随机数种子
              * @note 取值范围为 [first, last)
              * @param [in] first 随机数种子散列值起始位置
              * @param [in] last 随机数种子散列值结束位置
              */
             template <class It>
             void init_seed(It &first, It last) {
                 core_.init_seed(first, last);
             }
 
             /**
              * 产生一个随机数
              * @return 产生的随机数
              */
             result_type random() { return core_(); }
 
             /**
              * 产生一个随机数
              * @return 产生的随机数
              */
             result_type operator()() { return random(); }
 
             /**
              * 产生一个随机数
              * @param [in] lowest 下限
              * @param [in] highest 上限(必须大于lowest)
              * @note 取值范围 [lowest, highest)
              * @return 产生的随机数
              */
             template <typename ResaultType>
             ResaultType random_between(ResaultType lowest, ResaultType highest) {
                 if (highest <= lowest) {
                     return lowest;
                 }
                 result_type res = (*this)();
                 return static_cast<ResaultType>(res % static_cast<result_type>(highest - lowest)) + lowest;
             }
         };
 
         // ============== 随机数生成器 - xoshiro 算法(比梅森旋转算法消耗更少的内存，但是循环节更小，随机性比taus好) ==============
         // @see http://xoshiro.di.unimi.it
         // 循环节： 2^128 − 1
         typedef random_manager_wrapper<core::xoshinro_engine_128<false> > xoroshiro128_starstar;
         // 循环节： 2^128 − 1，少一次旋转，更快一点点
         typedef random_manager_wrapper<core::xoshinro_engine_128<true> > xoroshiro128_plus;
         // 循环节： 2^256 − 1
         typedef random_manager_wrapper<core::xoshinro_engine_256<false> > xoshiro256_starstar;
         // 循环节： 2^256 − 1，少一次旋转，更快一点点
         typedef random_manager_wrapper<core::xoshinro_engine_256<true> > xoshiro256_plus;
     } // namespace random
 } // namespace util
 
 #endif /* _UTIL_RANDOM_GENERATOR_H_ */
 
 
 // ================================= 简易单元测试 =================================
 #include <cstring>
 #include <iostream>
 #include <list>
 
 #ifdef max
 #undef max
 #endif
 
 // #include "random/random_generator.h"
 
 // ===================== 这里是单元测试组件的简化版 =====================
 #define test_case_func_name(test_name, case_name) test_func_test_##test_name##_case_##case_name
 #define test_case_obj_name(test_name, case_name) test_obj_test_##test_name##_case_##case_name##_obj
 
 #define CASE_TEST(test_name, case_name) \
 static void test_case_func_name(test_name, case_name) (); \
 static test_case_base test_case_obj_name(test_name, case_name) (#test_name, #case_name, test_case_func_name(test_name, case_name)); \
 void test_case_func_name(test_name, case_name) ()
 
 struct test_case_base;
 static std::list<test_case_base*> g_all_tests;
 static test_case_base* g_current_test;
 static size_t g_success_count = 0;
 static size_t g_failed_count = 0;
 
 struct test_case_base {
     typedef void (*fn_t)();
     test_case_base(const char* test_name, const char* case_name, fn_t fn): test_name_(test_name), case_name_(case_name), fn_(fn) {
         g_all_tests.push_back(this);
     }
 
     const char* test_name_;
     const char* case_name_;
     fn_t fn_;
 };
 
 #define CASE_EXPECT_EQ(L, R) \
 if ((L) == (R)) { \
     ++ g_success_count; \
 } else {\
     ++ g_failed_count; \
     std::cerr<< "Expect Failed: "<< #L<< " == " << #R<< " @"<< __FILE__<< ":"<< __LINE__<< std::endl; \
 }
 
 #define CASE_EXPECT_NE(L, R) \
 if ((L) != (R)) { \
     ++ g_success_count; \
 } else {\
     ++ g_failed_count; \
     std::cerr<< "Expect Failed: "<< #L<< " != " << #R<< " @"<< __FILE__<< ":"<< __LINE__<< std::endl; \
 }
 
 #define CASE_EXPECT_TRUE(EXPR) \
 if ((EXPR)) { \
     ++ g_success_count; \
 } else {\
     ++ g_failed_count; \
     std::cerr<< "Expect True Failed: "<< #EXPR<< " @"<< __FILE__<< ":"<< __LINE__<< std::endl; \
 }
 
 // ---------------------- 这里是单元测试组件的简化版 ----------------------
 
 CASE_TEST(random_test, xoroshiro128_starstar) {
     util::random::xoroshiro128_starstar gen1;
     gen1.init_seed(0x12345678);
 
     uint32_t checked1[] = {0x6baaab3a, 0x9385fdbb, 0x99ae85fe, 0x88e2358c, 0x6592b0ce,
                            0x20eadc84, 0x6a8d80b7, 0xa676abb7, 0x726ef19a, 0xd4e33183};
 
     for (size_t i = 0; i < sizeof(checked1) / sizeof(checked1[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked1[i]);
     }
 
     gen1.get_core().jump();
     uint32_t checked2[] = {0xb40ef1a3, 0xeaf5f926, 0xdaedd531, 0xd75a0342, 0xf5cd493d,
                            0x4081f259, 0x92a08a09, 0x8e7856ff, 0x3eecdb2,  0x34cd820c};
 
     for (size_t i = 0; i < sizeof(checked2) / sizeof(checked2[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked2[i]);
     }
 
 
     uint32_t  arr_seeds[] = {0x01, 0x02, 0x03, 0x04};
     uint32_t *ptr_seeds   = &arr_seeds[0];
     gen1.init_seed(ptr_seeds, ptr_seeds + 4);
 
     uint32_t checked3[] = {0x483919e4, 0xa90f4ce8, 0xe55d1499, 0x17c2185e, 0xdb11c3f7,
                            0x63557f98, 0xc7ad6874, 0x4a4a45ca, 0x74dec471, 0xf7e82f2a};
 
     for (size_t i = 0; i < sizeof(checked3) / sizeof(checked3[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked3[i]);
     }
 
     gen1.get_core().jump();
     uint32_t checked4[] = {0xf30187c0, 0xe533ff68, 0x628920df, 0xa998b9e7, 0xcac83081,
                            0xe23e175b, 0x8aef7f7a, 0x43e56ba2, 0x59dc108b, 0x35b5536c};
 
     for (size_t i = 0; i < sizeof(checked4) / sizeof(checked4[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked4[i]);
     }
 }
 
 CASE_TEST(random_test, xoroshiro128_plus) {
     util::random::xoroshiro128_plus gen1;
     gen1.init_seed(0x12345678);
 
     uint32_t checked1[] = {0xf1c950fb, 0x3c9b4647, 0x212ab3fb, 0x9930ad20, 0xe891a595,
                            0x5f3d9875, 0x264dd138, 0xb23b09e7, 0xb351ccd6, 0xf6342d85};
 
     for (size_t i = 0; i < sizeof(checked1) / sizeof(checked1[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked1[i]);
     }
 
     gen1.get_core().jump();
     uint32_t checked2[] = {0x65981904, 0x2d406a8b, 0xf722cc27, 0xd93f4e0,  0xc8c324e,
                            0x47adc6fc, 0x2e331b89, 0x2de59177, 0x68298f16, 0x3c1f786};
 
     for (size_t i = 0; i < sizeof(checked2) / sizeof(checked2[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked2[i]);
     }
 
 
     uint32_t  arr_seeds[] = {0x01, 0x02, 0x03, 0x04};
     uint32_t *ptr_seeds   = &arr_seeds[0];
     gen1.init_seed(ptr_seeds, ptr_seeds + 4);
 
     uint32_t checked3[] = {0x6ba5dc24, 0x1f1d11fa, 0xfa4932ff, 0x86a802af, 0x784c234,
                            0xbabe3a02, 0x77ad7d11, 0x7ebe938,  0x44211069, 0x2a5ba899};
 
     for (size_t i = 0; i < sizeof(checked3) / sizeof(checked3[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked3[i]);
     }
 
     gen1.get_core().jump();
     uint32_t checked4[] = {0x4956e25b, 0x1637a896, 0xc303f2ef, 0x9f3f6ac7, 0x8048281d,
                            0xfd181cdb, 0xd50c6ccb, 0xa1e5dc16, 0xe2e6feed, 0xe8901128};
 
     for (size_t i = 0; i < sizeof(checked4) / sizeof(checked4[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked4[i]);
     }
 }
 
 
 CASE_TEST(random_test, xoshiro256_starstar) {
     util::random::xoshiro256_starstar gen1;
     gen1.init_seed(0x12345678);
 
     uint64_t checked1[] = {0x83ea18ca6bf49f0c, 0xca24436623484287, 0xe7965c078396b9dc, 0xb9036864adb81725, 0x5c35ba435d92ab9b,
                            0x45202e3b4085a2cc, 0xa1e9410f7f528e4e, 0x48eb35365566f0b2, 0x90412e6e9ae131c3, 0xba4a1df61ca0b237};
 
     for (size_t i = 0; i < sizeof(checked1) / sizeof(checked1[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked1[i]);
     }
 
     gen1.get_core().jump();
     uint64_t checked2[] = {0x555e786541686a07, 0x6b5e064426c7dce4, 0x63a8537c3e2de537, 0x1b1eb7c6ef11c6c,  0x9d6eaac62ce6a4f4,
                            0x8b2cb439dc19fadd, 0xb749c288597ceac4, 0x92da83e146fdff1d, 0x110d38c051cededc, 0xadab1aacca67bd98};
 
     for (size_t i = 0; i < sizeof(checked2) / sizeof(checked2[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked2[i]);
     }
 
     gen1.get_core().long_jump();
     uint64_t checked3[] = {0x855835566240e421, 0xef37ac417209f23c, 0x27d0b120fbc1263b, 0x3acf06c9c8bebcfc, 0x8e4c39858e961d77,
                            0x54ef99ad7133b336, 0x507e39520e592971, 0xf17538afb5568bf2, 0x9b12bf7bc0ad1cb,  0xe30b952ddb0edc22};
 
     for (size_t i = 0; i < sizeof(checked3) / sizeof(checked3[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked3[i]);
     }
 
 
     uint64_t  arr_seeds[] = {0x01, 0x02, 0x03, 0x04};
     uint64_t *ptr_seeds   = &arr_seeds[0];
     gen1.init_seed(ptr_seeds, ptr_seeds + 4);
 
     uint64_t checked4[] = {0x167894d082017430, 0x38b95133d3ac9e80, 0x1f5b58670bd33b,   0xa72aa9c1f3c234dc, 0xa369742bfe107109,
                            0xa10057c6cbaaf5b7, 0xed9d205bdc0671c4, 0x36379ae97a7e84fc, 0x68a4d0807b200c28, 0x1178895e2d328ef3};
 
     for (size_t i = 0; i < sizeof(checked4) / sizeof(checked4[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked4[i]);
     }
 
     gen1.get_core().jump();
     uint64_t checked5[] = {0x6892346243ec2224, 0x721f3bb7498cd45b, 0x4706ddfc3ac5a535, 0x1833b360cae1f78f, 0x49c783132a986f0f,
                            0x228581842db7c171, 0x575b41c3f6bafa53, 0xd54e667c5bdc8331, 0xd16f385b2194a5ee, 0xd35320c4a01add19};
 
     for (size_t i = 0; i < sizeof(checked5) / sizeof(checked5[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked5[i]);
     }
 
     gen1.get_core().long_jump();
     uint64_t checked6[] = {0xfc07bbe2a0ff49e3, 0x25b74d3c3e1395a4, 0x66c3b4e434a41253, 0xeef93c334db407df, 0xcbe33255433c267a,
                            0x1aeb5a580f8b97f7, 0xee0b16ebb05cc830, 0x1951fff956477d9e, 0xd586fc5de6068234, 0xb77c43707de92854};
 
     for (size_t i = 0; i < sizeof(checked6) / sizeof(checked6[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked6[i]);
     }
 }
 
 
 CASE_TEST(random_test, xoshiro256_plus) {
     util::random::xoshiro256_plus gen1;
     gen1.init_seed(0x12345678);
 
     uint64_t checked1[] = {0xa6766515ded04cf7, 0x8b8ef459334146d2, 0xa5e132765ffaea1b, 0x42a32c7899960f6,  0xb55425c32b032ae,
                            0x4f5b510e66ed35c3, 0x86364d228b55294e, 0x15fee24568b9f2e4, 0x79172607cb7639c8, 0x380558a4c62a5df3};
 
     for (size_t i = 0; i < sizeof(checked1) / sizeof(checked1[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked1[i]);
     }
 
     gen1.get_core().jump();
     uint64_t checked2[] = {0x65e286c2682c7ede, 0x6fd589cda019d6da, 0x74ca58c461d9116,  0xaf78ff6a0b528fa,  0x532d4519bc355dff,
                            0x4fd9664f92848fd0, 0xe871531860e04f51, 0x9f66375cd3511195, 0xedb2c3bd1fad482d, 0xabd051255c4a95a4};
 
     for (size_t i = 0; i < sizeof(checked2) / sizeof(checked2[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked2[i]);
     }
 
     gen1.get_core().long_jump();
     uint64_t checked3[] = {0x823dc8ffe9ec3044, 0x2282021c3619f998, 0x58b7e7b6e4bf12c2, 0xba93e7ae8ffd60be, 0x7c5604182a5e5c,
                            0xfe2b1a4e6cc03509, 0x3cc13d59418b58b6, 0xee043f7276571ff3, 0xb0cefcacf43f423d, 0x2c06bd7d4b2a7b61};
 
     for (size_t i = 0; i < sizeof(checked3) / sizeof(checked3[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked3[i]);
     }
 
 
     uint64_t  arr_seeds[] = {0x01, 0x02, 0x03, 0x04};
     uint64_t *ptr_seeds   = &arr_seeds[0];
     gen1.init_seed(ptr_seeds, ptr_seeds + 4);
 
     uint64_t checked4[] = {0xf7d9f594a5851eca, 0x1554ccd2906e61d5, 0xf1a8da801e3b1052, 0xbf31cf72a94cc91b, 0x1a8b65873ac842c9,
                            0xd15befe036b53136, 0x8f8ff0898e5cca09, 0xf287e57ae52130ec, 0x4b93d1235a7a9687, 0x4a1e76c36a3f44f9};
 
     for (size_t i = 0; i < sizeof(checked4) / sizeof(checked4[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked4[i]);
     }
 
     gen1.get_core().jump();
     uint64_t checked5[] = {0x5ab99b574ab79179, 0x9dd72e9427eecf2d, 0x847348252d253c3a, 0x45e4172e49119e02, 0xfef4edb4819f165e,
                            0xecd2f9db3f6911a3, 0xaa21cb67c76ca6cf, 0x26a7e2345c58b741, 0xaf507d5180f1070c, 0x5c635df444a4f1};
 
     for (size_t i = 0; i < sizeof(checked5) / sizeof(checked5[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked5[i]);
     }
 
     gen1.get_core().long_jump();
     uint64_t checked6[] = {0x7f463ee9f76cdf70, 0xc0a587ad5ee6ec85, 0xb42e076175295e3a, 0x96d379025346fa5b, 0xa4de52d9b1d5e2e5,
                            0xa8790f06ce0d31de, 0x5d0eb9a768b4700f, 0xa4fbadd2392670bd, 0xc7b31a2f3b7819ee, 0xcca74cb594188bcd};
 
     for (size_t i = 0; i < sizeof(checked6) / sizeof(checked6[0]); ++i) {
         CASE_EXPECT_EQ(gen1.random(), checked6[i]);
     }
 }
 
 
 int main() {
     size_t success_count = 0;
     size_t failed_count = 0;
     
     for(std::list<test_case_base*>::iterator iter = g_all_tests.begin(); iter != g_all_tests.end(); ++ iter) {
         g_current_test = *iter;
         size_t old_fail = g_failed_count;
         size_t old_succ = g_success_count;
         std::cout<< "Start Test: "<< g_current_test->test_name_<< "."<< g_current_test->case_name_<< std::endl;
         g_current_test->fn_();
         if (old_fail < g_failed_count) {
             ++ failed_count;
             std::cerr<< "Test Failed: "<< g_current_test->test_name_<< "."<< g_current_test->case_name_<< " with "<< 
                 (g_failed_count - old_fail)<< " failed test(s) and "<< 
                 (g_success_count - old_succ)<< " passed test(s)"<< std::endl;
         } else {
             ++ success_count;
             std::cout<< "Test Passed: "<< g_current_test->test_name_<< "."<< g_current_test->case_name_<< " with "<< 
                 (g_success_count - old_succ)<< " passed test(s)"<< std::endl;
         }
     }
 
     std::cout<< "All test case(s) finished, success: "<< success_count<< ", failed: "<< failed_count<< std::endl;
     return 0;
 }