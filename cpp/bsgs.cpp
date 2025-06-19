// Basic implementation of BSGS using GMP, would be helpful for CUDA code ahead.
#include <stdio.h>
#include <vector>
#include <gmp.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <openssl/sha.h>

// a,g,p are only ones needed
// takes so much storage, obviously because I am storing such big strings, but I should store hashes of them simply
// but with standard hasher maybe some collision, not sure 

std::string sha256(const std::string &str){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    
    std::string output;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        output += buf;
    }
    return output;
}

std::string sha256_mpz(const mpz_t val){
    char *cstr = mpz_get_str(NULL, 10, val);
    std::string str(cstr);
    free(cstr);
    return sha256(str);
}

void bsgs(std::unordered_map<std::string, long long int> &lookup_table, const long long int m, const mpz_t a, const mpz_t g, const mpz_t p)
{
    mpz_t e, gm, gm_inv, a_val, m_pow, ival, m_2, jval;
    mpz_init(e);
    mpz_init(gm);
    mpz_init(gm_inv);
    mpz_init(ival);
    mpz_init(jval);
    mpz_init_set_ui(m_2, m);
    mpz_init_set(a_val, a);
    mpz_init_set_ui(m_pow, m);
    mpz_set_ui(e, 1);
    // Baby-step
    for (long long int i = 0; i < m; i++)
    {
        char *key_cstr = mpz_get_str(NULL, 10, e);
        std::string key(key_cstr);
        free(key_cstr);
        lookup_table[sha256_mpz(e)] = i;
        // e = (e * g) % p;
        mpz_mul(e, e, g);
        mpz_mod(e, e, p);
    }

    // maxsize is bigger than 2^25, what we need so it should be fine
    // RAM is the consideration here for my device :(

    mpz_powm(gm, g, m_pow, p); // gm = g^m mod p
    mpz_invert(gm_inv, gm, p); // gm_inv = (g^m)^-1 mod p

    std::string key_to_find = sha256_mpz(a_val);
    for (long long int i = 0; i < m; i++)
    {

        if (i % 100000 == 0)
        {
            printf("i = %lld\n", i);
        }

        if (lookup_table.find(key_to_find) != lookup_table.end())
        {
            long long int j = lookup_table[key_to_find];
            mpz_set_ui(ival, i);
            mpz_set_ui(jval, j);
            mpz_mul(ival, ival, m_2);
            mpz_add(ival, ival, jval);
            char *result_cstr = mpz_get_str(NULL, 10, ival);
            std::string result(result_cstr);
            printf("r  %s \n", result.c_str());
            free(result_cstr);
            break;
        }
        // a_val = (a_val * gm_inv) % p;
        mpz_mul(a_val, a_val, gm_inv);
        mpz_mod(a_val, a_val, p);
        key_to_find = sha256_mpz(a_val);
    }
}

int main()
{
    // we would assign them from strings
    unsigned long int n = 2;
    std::string a_str, g_str, p_str, m_str;
    a_str = "6384201945364259416484618556230682430992417498764575739869190272523735481484018572812468821955378599176918034272111105002324791003253919162436622535453745408437647881572386470941971475251078850286304439754861599469147475492519769292883229128057869632295023589669156129147001420291798187153143127735238126939728677098865317467404967501011186234614072662215754693860629617912797573819290964731520795401609222935030001060869435851177399452933354698474411159337923418076284460501174419894324660021273635203791996633586742144073352269865971941687673123777993188268979508244658784407075950581524330447222535111673938658510";
    g_str = "21744646143243216057020228551156208752703942887207308868664445275548674736620508732925764357515199547303283870847514971207187185912917434889899462163342116463504651187567271577773370136574456671482796328194698430314464307239426297609039182878000113673163760381575629928593038563536234958563213385495445541911168414741250494418615704883548296728080545795859843320405072472266753448906714605637308642468422898558630812487636188819677130134963833040948411243908028200183454403067866539747291394732970142401544187137624428138444276721310399530477238861596789940953323090393313600101710523922727140772179016720953265564666";
    p_str = "21847359589888208475506724917162265063571401985325370367631361781114029653025956815157605328190411141044160689815741319381196532979871500038979862309158738250945118554961626824152307536605872616502884288878062467052777605227846709781850614792748458838951342204812601838112937805371782600380106020522884406452823818824455683982042882928183431194593189171431066371138510252979648513553078762584596147427456837289623008879364829477705183636149304120998948654278133874026711188494311770883514889363351380064520413459602696141353949407971810071848354127868725934057811052285511726070951954828625761984797831079801857828431";
    // const int m = 65536;
    m_str = "33554432";
    // initialize with 0 currently

    // these are the bsgs_trial.py values for trial
    // a_str = "1307";
    // g_str = "2";
    // p_str = "10957";
    // m_str = "105";
    long long int m = std::stoll(m_str);
    mpz_t a, g, p;

    mpz_init(a);
    mpz_init(g);
    mpz_init(p);

    // set their values from strings here
    mpz_set_str(a, a_str.c_str(), 10);
    mpz_set_str(g, g_str.c_str(), 10);
    mpz_set_str(p, p_str.c_str(), 10);

    // hash table with string and index, for bigger values string and string
    std::unordered_map<std::string, long long int> lookup_table;

    bsgs(lookup_table, m, a, g, p);
    std::string trial = sha256(a_str);
    // printf("SHA256 of a: %s\n", trial.c_str());

    return 0;
}