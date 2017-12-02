#include <unity.h>
#include "../iota/transaction.h"
#include "../iota/conversion.h"
#include "../iota/addresses.h"

void test_CalculateStandardBundleHash()
{
    uint64_t timestamp = 1509635174;
    const char bundle_hash[] = "JBWYAJTGFDHWRBAEHPSKIWTGDHIMBGHEBEYSLFUHX9IBW9SB9SFLRUWIXHTUDCOERDLCHQXCQJCHTIINY";
    const char tag[] = "TREZOR999999999999999999999";
    const char to_addr[] = "WJWFNLVTTFIFDFSBUWKBEFCHEEBTAXM9HVJJXJXV9DPVLSLPYLKU9CRFHESHEJZYIGWBNI9UYYWFXRJHD";
    const char from_addr[] = "MC9ZFTZBG9YGNDSLRWUVWZZXMZVJEZZMRWWCMQTG9HBEPTGSEEBMJX99U9UTP99EZZLX9HHACDNFWWTSX";
    const char remainder_addr[] = "JBKIFPLXDQGPAOA9IHSHHYPQGNHANBOHOEZDHSEXRSJAOHCJ9YTUDVNMFZMGCOXOVJIJRXSFKYOXILCZY";

    tryte_t hash_out[81] = {0};
    calculate_standard_bundle_hash(from_addr, to_addr, remainder_addr, 10000, 2001, tag, timestamp, hash_out);

    // Do a conversion to chars for easier comparison
    char hash_out_chars[82] = {0}; // one extra zero as string termination
    trytes_to_chars(hash_out, hash_out_chars, 81);
    TEST_ASSERT_EQUAL_STRING(bundle_hash, hash_out_chars);
}

void test_NormalizeHash()
{
    const char hash_in_chars[] = "VEJURZLOSF9ZPCOQIILAIKMLCUUCKNEFZOVIGKEOLDVOTSA9SVSTVWJYFCZIYAUIYVD9ABQZHXYFC9PJX";
    const char expected_normalized_hash_chars[] = "NNHURZLOSF9ZPCOQIILAIKMLCUUMMMEFZOVIGKEOLDVOTSA9SVSTVWNXFCZIYAUIYVD9ABQZHXYFC9PJX";

    // Convert to trytes first
    tryte_t hash_in[81];
    chars_to_trytes(hash_in_chars, hash_in, 81);
    tryte_t normalized_hash[81];

    // Call function
    normalize_hash(hash_in, normalized_hash);

    // Going to compare in chars, converting output to chars
    char normalized_hash_chars[82] = {0};
    trytes_to_chars(normalized_hash, normalized_hash_chars, 81);

    TEST_ASSERT_EQUAL_STRING(expected_normalized_hash_chars, normalized_hash_chars);

}

void test_SignatureGeneration()
{
    const char first_sig_expected[] = "BVRACHWMMSVSDOJJZQKDAQOODUIWIKYEKXVMJRMJUFPE9EAXVMXKTFFBTAOJTR9IKFYHMPOHGS9OTVWWDCREBLGHMVSGUKKWGDVIIHMTFKZPYKCKUVXTF9WQDVTYQWEHLXBXKUHQAXV9HTEWDMUWCTQRMXFXRLBJXW9LLYNRXUYAYDOJXPPWXLLFQNR9PRQTVSECLNWIUJSWRLZDWUTXHCZMMNZV9OBSYLANTFPOKRZVT9AAUHDCZIYJHEVXRJEUJPZANFIYROUEC9QMXNRMCXVMXXWXPEIJTIQOKNSWXCAFBWLUKOOCDQFCTQMPREXGJP9DNSZMJXVYWUUUAWZQBBFJQZRIZFXUAHUNCYRAGPUCWQEPPZXOHZ9YAQ9DBUQNA9ZVJFCEOQA9XSMBIGMUCUSBHLTTZBMPTMGCMFNQZCYGBTQZHRBP9LANEOWKRHUPYSXVNNBCJXWHZKWWDOZFAGEIZCCXYUOBCZEGQACZVBTTOYFLMQLTXJHSOCDJ9JOSAHMYFOUBWJYERST99PPVDRENHSAPIQOCLVUGQQYBTIBRKFJDL9MNMSWUXMPMJCXODJEXQSXIGIGZHYLACIZZKOXAIFHZXHWUZVUJUUTJADOBRFOYBUNWJYTQDWUOUOWDBLVYAQVWJOUNPW9LSULAFUPQQJHMANAXZRCODRXTGRANFF9WMNRAVKOHOJUSASVWTDTOADYAVQICNFVXAHKJDEIZZECRHF9LBJFAJUXMKXVOTEPYVYIZMJMBTFUHWRH9NTYOZQWMSIDWRUXYJ9LUNVGABLFLNOVQFZPYNUXILWZMMEC9QNAEYGBOZILZJLNYTUDJAASIPAGWHPIE9JX9LTNDKTBOAHUGITQPQEGANSOAETCU9KYZLWMGRRYELOLXKLZSBO9BJLOGDWAY9XIYCELKGPMIPYNM9DHWIJCGAKKFLUATFJJNJZSXRYHESID9T9XVS9EKJEND9TOSDFGWUBMJAWIOPVCVCUFPOSPZNMTATFYIHEAKPUCUMFCRQPAHZQAGQWU9ENAKWSCVMSVTICPBGDGXCBCTHVSL9QEPPQGEZVO9RDIUIGNWWIBMQMWYXEYAAMXSNB9ESPTMJ9EJJIYFBDAZQOCBIHDPEGKXKJLJMWV9H9NUU9NPAIKSESVETWSXHYEPGKBVXKKBIDHUDXFZASMIXSTBLBSTNRKADXCHKXGGOYFSFRPVQXAHYKYKLNMNOZPIXVPOZMNACHCCMYANGUBYFIACXNGIVURMCVQO9VULAALLWRHIWEKXYTSVCQDCIJJOKTYTIAO9UVFRRIJUKXWX9XQHPEUPBBSH9DNBWNYLUWUK9CCFDRKIFXVWPVXZVGPVFQJZEDQYDLRTFWELZWNU9QSM9CCPCEV9XISXRBKFEAVVQH9GCFXXFVHIMZKGKKXROOWMAGDXOMMBBALALDQQ9AZZGMXZSMAQIEFQIRFHNCVFPDJWIUQGSPYCYPFCQV9KUDSIMDMMTNXLSSBJQ9NXNXZITDZKHTLNAQGSGPAKABQRLTKVJEPKLVOOHC9WHGINTETERMBGVSECAAUYEMTXMYNUZKFOXQFRVOZMFOLPVFRZTKZ99ZVOMISIDVZGRZQKDYIBTLUQSRCCIF9ZIGAQYXIOPJYRKKWPHQCXHSDFSZRZOYUKXIKJSXMCVXBITYQOZMVBXAOD9MIDLLVWGCEEPOBEJQBMCYNDWHDCHJKIDCFGZMPCMGIKOKWAIQBVNKEZDCNZRLOURQDZDKNUHMRNJHEUAHKRVZRWV9RUWVTTVYYHGAPNRVPYVUMHPEPRQXWGQRA9TCEQEQKMYEQO9RYPCAEGWMDFODWSECDEZZFVFHLVSKPXDZTARUVDNSLW9PDWNMRECKMB9EVUZIUSGDDVSKIGJLTZIXWYELPHEWWPWD9YFDAEEWIZOZHPBC9VKVVTMJQOQTVGAQINI9MDAYJLFVCRMALQGQBJCLVX9URZSUO9JTESN9WFAZFS9RAJWXMQZGYVWHHXFXCQLOEOE9GQH9MWGNEMIGLCYXCFLVXHMQIBGZXRQ99ZXLWA9MCJVMYNGLTNDLRZZMANGDNLAAKWSTGLBMSGGBTALWZHGRKQZSKYOVUPDW9ZDRCXUX9PRFT9XLJKYOMADAYZ9DOFTVGYNLVJSAEHWVQHLZBMXSKROCEG9ZNTSDYDSXDYXWSIWSISHEW";
    const char second_sig_expected[] = "BTPPIPJTUTOIIHWSMRHW9BATMAZBXTMLBBOTYROOYYLKDTDSHQYCEHIHBKANZISRXZTCONXWULWEQACBAVQCCLERPSYXRXSYIFFBLLMEJVZEHZZCCHPQXRDYZHAIDAMREN9OVHENIYFMMWJRKKOGUIKZXU9ERQJZOXRIDSZZOQPXNUXXYCMCRTNRDSMAEJYUYSJNBETHHVODZZZSTJWMCRWISMZOGPNTMJ9ESZVLQPPQTHDYVWCAEAALOSANUYMPUMERMRKECUEW9TRRKHIMRHN9ILPHKYUSZFMRHNWQJD9LDZHBYH9VDZWTYVSZQKNFMCWWGJDJHJTDKO9UQD9HFTIPHVJFINBYXPJRWZADGTTSBEY9RLCRUDRQKZTOZPRDBXLFSSTNGHVXU9KZGJGBDKVBIFWYLIDSXMGBKWSIRQ9ATPNCNYEFPSVCTSXOZBFQHZB9YNTOYKYHRSRVRZDFHOXTGJRLHOANSMMZFDKUHHTBP9RHVJYUQ9CWDEBOYKKFMPDXA9ZIYWRPF9ZLCIDPRAA9FYGMQABJZLNUHIEOGMHSJYWCPTRGPZ9IKTYMEWB9JTWTHQZE9EVVHUHBMLRCGHRZLWCVTPFUWKAJJDHTKTGZTRWYPCGUFYCHWTIZZAOZHXKBNYYZRKFVUAGEQMWTYTQAGDIASCUL9MBBQAVBWOJWZHVXNAVRYOOGJHPWGBGEGBTVZRX9RQUDYVTHRI9XOXXLCIVMXVSADZVJOKMSSHWKGTBMFDYM9NDAVLIWILFAIMYFFRPPOUCZURQ9N9NRTZFTCOEHKVCKZKGGPOFTI9RNSAZSOSYABOUHTLUDC9YDRGSBSRPTCIZMSCHNIKJYSDHDDQIEPHFEZWXREOSK9VSJYBNUCZRZJSOPKGDUDFZF9BMMOCZFLZXVXBFANEWDYWZOI9YJJFPKUEVLZDUYYNJPWSVXCRIFZNOVJTRGCKILYDLUTNMKYJBXYXORNETUZCBQWEBBWVOHNGUBU9SUFBCVRH9HMMGVAIIMNHA9IINLVRCAUHXKOZCXOOXOWVFLUWXMZLMQDFRFOQSKSFRRFQUNDWNATVQGAYONLVDPYRAMSJDTFUGKNBMZVVRFWB9EJTX9PFNSPRBK9FXHQODXPVBIMZODIQZEBPMLVROKVLWOFEYFZOYEADZWAMQAEFZGQNNQSBGEGDMSOYICVAMSJNQ9HIWH9TGHXMILGWMWSHDFIPWKJOZPXF9PUPOWCKAVVTUSVQLRKENTQDXVKJMYDCUYNV9YMFNKJLSWNRXPFOGWOOF9WAYTVCWMRFRCDYRXMICMVKWDEGXXQOLHCUXX9OXJIQAIGJY9HJXCBTEMBAXAWIIKDMAXKNJBF9VXZRBVXKWHBTOUHIVJZPMEY9UVWYGHR9SPEDXCWESTBRNYZNUQZAGYREJZRMUBKMYVKZVHBVYGSQJIGZJHCAVBWEDQGVHDRKICXXNUVLYUQNLGWIRYMHP9CYHFMNUIUJZDWIFKZYFKSLCSWVZVODFJAHNMRLCRVLGNEFABNC9BBMAYZXHJQKDU9KA9FKAXCUDDTZWHCVZGA9JGQAQ9WOOQYONEGRLMFVT9HQGTZEYHLIEFZNJYLKXKDQURZH9TKFEZQKUCU9XRKOQJSXYW9WLPOXURVGVJRXFZDLCMCDYXMHA9YPSXKBNVQHMOYQIIGBJZUXNSWLY9NPOTAEOOATEGXJKEUGLIBRKQEVMC9NGUGBEGOCKNZCWPDOUOVPIJUNQQPCJTJRNBNILPAECLKNZBKIZLCWEUFOPXP9CSPDLQHILFEONOHSV9RVXAFYIBVAOWY9NTWPSHE9RSZKDENTPXMEWOCOLYZCHWAG9LBHZQXHXEIPUJT9IV9AWLLNSEEPVJJZCUXXFOEAIRCDWMBYSVALCPVAIFRDEWGMESD9GXNSRGYYBDSGXYYHVKZYSOJJTWMHBPHHJ9EKJZRTQXIQBQTTJFPECZGPZJLGXTSWQVAQMGRDMJDMBYZHAHHDVMXTGFNJBGDCAMEB9NFVRAY9RPSSLQZGRYXYVSBWYLCJJJAJILAZFGWB9JXFRJV9HTLJRTVYILDZAKB99QOTXTXLUEDYNMETGTQYXBRHETGCGHCCXVQLFPULGRVWN9MKTEMLXLPGIR9GXLWYDSN9ATRHUJCGQOTCGJXTWIDHPXTJECKGHGRQPDIJCLTKZXEQX";

    const char test_seed[] = "SDF99AKJLWERYUASDFHUSADFHISDFIHOSADFIHJDASFLJDSFLKURYY9NDFSJHJIHJOERODDAKJLSANNND";
    const char normalized_bundle_hash[] = "NNHURZLOSF9ZPCOQIILAIKMLCUUMMMEFZOVIGKEOLDVOTSA9SVSTVW999999999999999999999999999";

    // Convert the seed to trits
    tryte_t test_seed_trytes[81];
    trit_t test_seed_trits[243];
    chars_to_trytes(test_seed, test_seed_trytes, 81);
    trytes_to_trits(test_seed_trytes, test_seed_trits, 81);

    // Create the private key
    trit_t private_key[2*243*27];
    generate_private_key(test_seed_trits, 0, private_key);

    // Convert the normalized bundle to trytes
    tryte_t normalized_bundle_hash_trytes[81];
    chars_to_trytes(normalized_bundle_hash, normalized_bundle_hash_trytes, 81);

    trit_t signature_fragment[243*27] = {0};
    generate_signature_fragment(&normalized_bundle_hash_trytes[0], &private_key[0], signature_fragment);

    // Convert the signatures to chars for easy comparison
    tryte_t signature_fragment_trytes[81*27];
    trits_to_trytes(signature_fragment, signature_fragment_trytes, 243*27);
    char signature_out[81*27+1] = {0}; // append a zero for string termination
    trytes_to_chars(signature_fragment_trytes, signature_out, 81*27);

    TEST_ASSERT_EQUAL_STRING(first_sig_expected, signature_out);

    // Try the second fragment too
    memset(signature_fragment, 0, 243*27);
    memset(signature_out, 0, 81*27);

    generate_signature_fragment(&normalized_bundle_hash_trytes[27], &private_key[6561], signature_fragment);

    // Conversion and comparison
    trits_to_trytes(signature_fragment, signature_fragment_trytes, 243*27);
    trytes_to_chars(signature_fragment_trytes, signature_out, 81*27);

    TEST_ASSERT_EQUAL_STRING(second_sig_expected, signature_out);
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_CalculateStandardBundleHash);
    RUN_TEST(test_NormalizeHash);
    RUN_TEST(test_SignatureGeneration);

    return UNITY_END();
}
