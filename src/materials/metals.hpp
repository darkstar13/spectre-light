#ifndef __METALS_HPP__
#define __METALS_HPP__

#include "utility/spectrum.hpp"
#include <initializer_list>

namespace metals
{
    class Metal
    {
        const Spectrum n;
        const Spectrum k;
    public:
        Metal(const std::initializer_list<float> ior,
              const std::initializer_list<float> abs):n(ior),k(abs){};
    };
}

const metals::Metal silver(
    {
        0.049347531050443649f,
        0.041602473706007004f,
        0.041009880602359772f,
        0.048415124416351318f,
        0.050000000745058060f,
        0.050000004470348358f,
        0.053292501717805862f,
        0.058362640440464020f,
        0.053611941635608673f,
        0.052341341972351074f,
        0.057906620204448700f,
        0.056908659636974335f,
        0.052224893122911453f,
        0.047666668891906738f,
        0.043222222477197647f,
        0.038946233689785004f
    },{
        2.2301845550537109f,
        2.4536008834838867f,
        2.6500174999237061f,
        2.8523058891296387f,
        3.0390980243682861f,
        3.2232837677001953f,
        3.4100761413574219f,
        3.5948910713195801f,
        3.7597548961639404f,
        3.9249725341796875f,
        4.0942010879516602f,
        4.2543230056762695f,
        4.4093589782714844f,
        4.5658330917358398f,
        4.7236108779907227f,
        4.8811230659484863f
    });

const metals::Metal Aluminium(
    {
        0.51354420185089111f,
        0.57037156820297241f,
        0.63335359096527100f,
        0.70365858078002930f,
        0.77591800689697266f,
        0.84938037395477295f,
        0.93028640747070312f,
        1.01519179344177250f,
        1.10655558109283450f,
        1.21029591560363770f,
        1.31434142589569090f,
        1.43030822277069090f,
        1.55902600288391110f,
        1.69751429557800290f,
        1.84196209907531740f,
        2.0029783248901367f,
    },{
        4.9599561691284180f,
        5.2085542678833008f,
        5.4542770385742188f,
        5.6952328681945801f,
        5.9313440322875977f,
        6.1647310256958008f,
        6.3964543342590332f,
        6.6272826194763184f,
        6.8546180725097656f,
        7.0752577781677246f,
        7.2957329750061035f,
        7.5080804824829102f,
        7.7104082107543945f,
        7.8930425643920898f,
        8.0657701492309570f,
        8.2151412963867188f
    });

const metals::Metal Gold(
    {
        1.46212887763977050f,
        1.44386053085327150f,
        1.38312280178070070f,
        1.30071735382080080f,
        1.10259127616882320f,
        0.80312025547027588f,
        0.55778276920318604f,
        0.42910495400428772f,
        0.34056714177131653f,
        0.27199321985244751f,
        0.22584775090217590f,
        0.18836064636707306f,
        0.15558262169361115f,
        0.13766665756702423f,
        0.13322222232818604f,
        0.13127875328063965f
    },{
        1.9556787014007568f,
        1.9458420276641846f,
        1.9123861789703369f,
        1.8580939769744873f,
        1.8412964344024658f,
        1.9728720188140869f,
        2.2039384841918945f,
        2.4696044921875000f,
        2.7156329154968262f,
        2.9560720920562744f,
        3.1913807392120361f,
        3.4033818244934082f,
        3.6024391651153564f,
        3.7917332649230957f,
        3.9721779823303223f,
        4.1496315002441406f
    });

const metals::Metal Copper(
    {
        1.28916776180267330f,
        1.25370144844055180f,
        1.24246585369110110f,
        1.24615097045898440f,
        1.22706675529479980f,
        1.19744002819061280f,
        1.12735247611999510f,
        0.99964505434036255f,
        0.81558209657669067f,
        0.60871571302413940f,
        0.38085740804672241f,
        0.27526932954788208f,
        0.23780889809131622f,
        0.21766665577888489f,
        0.21322222054004669f,
        0.21361127495765686f
    },{
        2.1880841255187988f,
        2.2994711399078369f,
        2.3922898769378662f,
        2.4754829406738281f,
        2.5438442230224609f,
        2.5888161659240723f,
        2.5977578163146973f,
        2.5913863182067871f,
        2.6581282615661621f,
        2.8192343711853027f,
        3.1063749790191650f,
        3.3725504875183105f,
        3.6263988018035889f,
        3.8538665771484375f,
        4.0574221611022949f,
        4.2534952163696289f
    });

#endif
