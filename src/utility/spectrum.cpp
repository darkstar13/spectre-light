#include "spectrum.hpp"
//lookup tables for spectrum to XYZ
//taken from http://www.brucelindbloom.com/index.html?Eqn_Spect_to_XYZ.html
#if HQ_SPECTRUM==0
const float X[SPECTRUM_SAMPLES] =
{
    0.048547909657160444f,
    0.24864331478873888f,
    0.33913105790813763f,
    0.23759311571717262f,
    0.068513086394717301f,
    0.0074335845497747266f,
    0.08596576422452927f,
    0.30151855826377871f,
    0.58514208267132439f,
    0.88403650492429731f,
    1.0476295638084412f,
    0.91729557037353515f,
    0.55824810008207959f,
    0.24846323480208715f,
    0.082662385882188882f,
    0.023565863414357107f
};

const float Y[SPECTRUM_SAMPLES] =
{
    0.0013634899475922187f,
    0.010440415943351884f,
    0.033597446996718648f,
    0.077808559965342278f,
    0.16970793592433134f,
    0.38263264827430249f,
    0.73335171341896055f,
    0.95049857179323827f,
    0.98971243401368458f,
    0.88304891208807623f,
    0.67035055945316946f,
    0.43439004709323248f,
    0.22639957609275976f,
    0.093565923050045963f,
    0.030277141408684354f,
    0.0085307513410225518f
};

const float Z[SPECTRUM_SAMPLES] =
{
    0.23185277074575425f,
    1.2145957302053769f,
    1.7617404375473658f,
    1.4557389440139135f,
    0.66370667571822806f,
    0.24021451229850452f,
    0.075098564475774759f,
    0.020824563254912696f,
    0.0045391401535986612f,
    0.0017035374639090151f,
    0.00090068979518643274f,
    0.00025727244249234595f,
    3.9191220954914268E-05f,
    1.964333174934533E-06f,
    0.f,
    0.f
};

const float INVY_SUM = 0.17546832144055843f;

const float spectrumWhite[] =
{
    1.0619347266616228f,
    1.0623373513955183f,
    1.0624330274817486f,
    1.0624850787200137f,
    1.0622213950288308f,
    1.0613081599651542f,
    1.0613058645182336f,
    1.0618168659745209f,
    1.0624642293010491f,
    1.0624838864140043f,
    1.0624682453762331f,
    1.0625355983287506f,
    1.0624016329348598f,
    1.0622653248789862f,
    1.060266533148627f,
    1.0600420908765831f
};

const float spectrumWhiteL[SPECTRUM_SAMPLES] =
{
    1.1560446394211681f,
    1.1564162465744781f,
    1.1567872929485827f,
    1.1565328954114107f,
    1.1565926830659454f,
    1.1565499678850697f,
    1.1472133116300325f,
    1.1314209727068025f,
    1.096408860125702f,
    1.0338718350511178f,
    0.96528604465789958f,
    0.92067216838305188f,
    0.90011672087937411f,
    0.88940075523174911f,
    0.88083842252481404f,
    0.87810499922653207f
};

const float spectrumCyan[SPECTRUM_SAMPLES] =
{
    1.0240953312699979f,
    1.0245612457188975f,
    1.0463755555238345f,
    1.0327846651059092f,
    1.0478428969483209f,
    1.0535090536305822f,
    1.0534870576691449f,
    1.0530663848751949f,
    1.0549102750144981f,
    0.94299173220279198f,
    0.3100097408600444f,
    0.0033711342032203243f,
    -0.0048549813110745684f,
    0.0018582205785167482f,
    0.0039837672915054804f,
    0.010507259067086385f
};

const float spectrumCyanL[SPECTRUM_SAMPLES] =
{
    1.1352399582424499f,
    1.1358531764433719f,
    1.1362707169771014f,
    1.1359364376354608f,
    1.1361867189829913f,
    1.135817770159788f,
    1.1359519356976406f,
    1.135423392708292f,
    1.1224513886352236f,
    0.87073337556349084f,
    0.3803441995397272f,
    0.051216852241201545f,
    -0.011762638745943615f,
    -0.01060685685959013f,
    -0.006931473364874461f,
    -0.0077818774183695668f
};

const float spectrumMagenta[SPECTRUM_SAMPLES] =
{
    0.99302530302633674f,
    1.0170691330352013f,
    1.0143947530476214f,
    1.0070517895374196f,
    0.80112726913173504f,
    0.077593476678434567f,
    0.003229957831351733f,
    -0.004352238640709956f,
    0.0026944590704797754f,
    0.28205531033673215f,
    0.8570353689334701f,
    0.99378492125784268f,
    0.98449588288224388f,
    0.8937980881442511f,
    0.94958431903872431f,
    0.9395992587226637f
};

const float spectrumMagentaL[SPECTRUM_SAMPLES] =
{
    1.0765584064227334f,
    1.0770490751029975f,
    1.0731253134738323f,
    1.0796647470180021f,
    1.0024747756009726f,
    0.4395828981593643f,
    0.02042973274257508f,
    -0.0015031343728669692f,
    -6.099749699375323e-06f,
    0.072151645981868115f,
    0.48078616824947817f,
    0.97313406556425108f,
    1.0781818622728534f,
    1.0327505540054573f,
    1.0495214724241742f,
    1.0257450908661028f
};

const float spectrumYellow[SPECTRUM_SAMPLES] =
{
    -0.0059362362867909409f,
    -0.0040293484704144403f,
    0.034632747920561285f,
    0.19407661745186114f,
    0.45561541868250915f,
    0.78117265145981962f,
    1.0163873556505527f,
    1.0511958466847318f,
    1.0513470268321483f,
    1.0515277720869929f,
    1.0512298920801075f,
    1.0515211534901903f,
    1.0514264026060656f,
    1.0513103386739624f,
    1.0507004197273715f,
    1.0485826837788901f
};

const float spectrumYellowL[SPECTRUM_SAMPLES] =
{
    0.0001468672999305493f,
    -0.00013161147654402951f,
    -0.00016768424395723818f,
    0.089519214436320216f,
    0.74821476916582985f,
    1.0340727288469598f,
    1.0365778653585402f,
    1.0367058054560021f,
    1.0365194490895373f,
    1.03661227107821f,
    1.0361321399468379f,
    1.0144985871415191f,
    0.8293751396865352f,
    0.6705682032005652f,
    0.60059597683336108f,
    0.58277723714307716f
};

const float spectrumRed[SPECTRUM_SAMPLES] =
{
    0.11487922506830811f,
    0.060141120462551691f,
    0.0040665397109191335f,
    0.010459427718803191f,
    0.0035470993579631675f,
    -0.0052706076654779289f,
    -0.0062588252221244959f,
    -0.0086496045197971341f,
    0.00097200190739861079f,
    0.14679380036909495f,
    0.85847180162874637f,
    0.99821493324988597f,
    0.99605297040670981f,
    1.0018494025816944f,
    0.99593834054491903f,
    0.9811979963396622f
};

const float spectrumRedL[SPECTRUM_SAMPLES] =
{
    0.057139392791085111f,
    0.043034047329456572f,
    0.021260689526515806f,
    0.001077172714861781f,
    0.00057985241220036873f,
    -0.00022486144117236386f,
    -0.00012009820021618776f,
    -0.0001991308173681336f,
    0.012756076079520295f,
    0.1832461591194777f,
    0.51948819108311795f,
    0.82120171360154059f,
    0.96263010562297358f,
    0.99410699787589729f,
    0.99018057306059759f,
    0.98278552726948454f
};

const float spectrumGreen[SPECTRUM_SAMPLES] =
{
    -0.010865527381003439f,
    -0.010329458431599345f,
    -0.0083431520558099291f,
    0.083794233190453149f,
    0.57500780803880425f,
    0.95115677422179923f,
    0.99948898769838934f,
    0.99968078182605802f,
    0.9988159758735875f,
    0.88618140828021486f,
    0.35690377193776984f,
    0.01325598457467465f,
    -0.0050991929756587905f,
    -0.0083927995026960873f,
    -0.0084413650357697944f,
    -0.0047501377518373699f
};

const float spectrumGreenL[SPECTRUM_SAMPLES] =
{
    0.0064830780912117957f,
    0.00019032331867488899f,
    -0.0081060480798639516f,
    0.048161890183585902f,
    0.66729637282872345f,
    1.0307844454225901f,
    1.0311600157417389f,
    1.0265626896736526f,
    1.0363099387922192f,
    1.0120735391513225f,
    0.32668720729539291f,
    0.0033846154767388065f,
    0.0081701266623202973f,
    0.0088889810224966476f,
    0.00036631914529600032f,
    0.00099462806143045101f
};

const float spectrumBlue[SPECTRUM_SAMPLES] =
{
    0.99498216185557875f,
    0.99569451590852531f,
    0.99983310193704411f,
    0.9648523926660395f,
    0.67060127526424484f,
    0.29157891777810102f,
    0.044614561825850822f,
    -6.7793271695393519e-06f,
    0.00050597357489660954f,
    0.0023497993510693772f,
    0.00067442519549839989f,
    0.016621955742817246f,
    0.040211692914411255f,
    0.049604490414015802f,
    0.043574051087547458f,
    0.027483432250758107f
};

const float spectrumBlueL[SPECTRUM_SAMPLES] =
{
    1.054236254920313f,
    1.0576206026996142f,
    1.058143833550661f,
    1.0568818098511983f,
    1.0207912014756255f,
    0.2974275399820579f,
    -0.0014770394250804989f,
    -0.0013982161133251694f,
    -0.00059190711447091779f,
    -0.0010090527379278194f,
    -0.0015479588813372375f,
    0.0051106864601078716f,
    0.047054873524993275f,
    0.12827536395203271f,
    0.15246421103968871f,
    0.16615733676564479f
};

const float spectrumOne[SPECTRUM_SAMPLES] =
{
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f
};

#else

const float X[SPECTRUM_SAMPLES] =
{
    0.024701516690453711f,
    0.076956649151260298f,
    0.2004350226997367f,
    0.31663273111947099f,
    0.34745464804672427f,
    0.32443142634726357f,
    0.26705865556113845f,
    0.16810038635006533f,
    0.078970335058827959f,
    0.025634237585579166f,
    0.0044517286485236032f,
    0.015246827755816908f,
    0.073215138207199507f,
    0.17365097249106995f,
    0.29548056108576698f,
    0.43423995379158242f,
    0.58934738685847532f,
    0.75095066614048478f,
    0.90105595533565852f,
    1.0129624876219738f,
    1.0593910847620298f,
    1.0183588273294513f,
    0.89162685539453246f,
    0.69872293195737267f,
    0.50269286377135092f,
    0.333828569036978f,
    0.20368577807901381f,
    0.11427220322792572f,
    0.061543243243008493f,
    0.032363974096113288f,
    0.016041421388396062f
};

const float Y[SPECTRUM_SAMPLES] =
{
    0.00068384978146309838f,
    0.0021800735059647447f,
    0.006851279475470819f,
    0.015728495002784289f,
    0.027907398512226414f,
    0.04454474458390819f,
    0.068175919229384971f,
    0.10231247686971251f,
    0.15286495945786913f,
    0.22809595458850454f,
    0.351284183069904f,
    0.53085674902764735f,
    0.72435610701319941f,
    0.86710343356094022f,
    0.95403010360335816f,
    0.99344854086317036f,
    0.99380680831850232f,
    0.95438353018094113f,
    0.87808588924587394f,
    0.77214066934841963f,
    0.65160071655627227f,
    0.52777666928787381f,
    0.40819105686840179f,
    0.29412212448014347f,
    0.19959081462634487f,
    0.12738475446378969f,
    0.075883360635288985f,
    0.042011186984237503f,
    0.02244464196364885f,
    0.01172945790974202f,
    0.0057965466687555809f
};

const float Z[SPECTRUM_SAMPLES] =
{
    0.11744817481886964f,
    0.36822380179679581f,
    0.96965829153535144f,
    1.561114181023771f,
    1.7696308739018694f,
    1.751946671150064f,
    1.5831404219006986f,
    1.1613441071491095f,
    0.72543093309928042f,
    0.42338677247845929f,
    0.25384485248756616f,
    0.14668675189857819f,
    0.075257753567389701f,
    0.040827568489357237f,
    0.020198238728447716f,
    0.0090112276398576704f,
    0.0041066267446274477f,
    0.0022205320697638283f,
    0.001668006851148831f,
    0.0011963934406277641f,
    0.0008662132330718253f,
    0.00044745843030727633f,
    0.00021118489270813886f,
    7.7521603363434416e-05f,
    2.6028387561695158e-05f,
    6.525415325240176e-06f,
    0.f,
    0.f,
    0.f,
    0.f,
    0.f
};

const float INVY_SUM = 0.090564294937062417f;

const float spectrumWhite[SPECTRUM_SAMPLES] =
{
    1.0616712525422225f,
    1.062216861387369f,
    1.0622480563302861f,
    1.0624500559277379f,
    1.0624228656416186f,
    1.0624454646387758f,
    1.0624897396026778f,
    1.0624747257007181f,
    1.0623154718763204f,
    1.0619023706321649f,
    1.0613248848992094f,
    1.0611029206436011f,
    1.0613192520971142f,
    1.0614225466093745f,
    1.0617994005892317f,
    1.0622916846111115f,
    1.062503384857943f,
    1.0624601372863196f,
    1.0625012980083177f,
    1.0624430254379029f,
    1.0624730546324526f,
    1.062534482259639f,
    1.0625386026158805f,
    1.0624541949301343f,
    1.0623802598916099f,
    1.062418516964351f,
    1.0622126493820012f,
    1.0610331975677827f,
    1.059705610510929f,
    1.0598857222005207f,
    1.060175206046359f
};

const float spectrumWhiteL[SPECTRUM_SAMPLES] =
{
    1.1563278767278407f,
    1.1557546536047163f,
    1.1562251245256241f,
    1.1566825367152893f,
    1.1567929502399881f,
    1.1567759749095703f,
    1.1565378301419382f,
    1.1565103556685912f,
    1.1566390897143104f,
    1.1565005072708163f,
    1.1564978084531539f,
    1.1558445030585924f,
    1.1485190260245597f,
    1.1376302294303986f,
    1.1311333113802877f,
    1.1292195881614477f,
    1.0988577735660499f,
    1.0505973314443511f,
    1.0359350056089058f,
    0.99396651814264092f,
    0.95978335030969208f,
    0.93180671964199113f,
    0.91808294758940878f,
    0.90627870685760936f,
    0.89764332214065701f,
    0.89322946495914346f,
    0.88737949617702561f,
    0.88232890008258602f,
    0.87991593719607586f,
    0.87724327576968775f,
    0.87898440245177745f
};

const float spectrumCyan[SPECTRUM_SAMPLES] =
{
    1.0193055796181778f,
    1.0286170826436831f,
    1.0147896255414286f,
    1.0369139609273794f,
    1.0440682963074026f,
    1.0501759309538792f,
    1.0337156776739727f,
    1.0296343836995636f,
    1.0469382957883808f,
    1.0528464785581175f,
    1.0535995237138822f,
    1.0534667820007202f,
    1.0536497781317902f,
    1.0530171006368012f,
    1.0529199014879445f,
    1.0540502008053367f,
    1.0551713924875086f,
    1.0525560958098032f,
    0.96317967585554132f,
    0.60841540341360056f,
    0.23747861610879942f,
    0.037207763343393999f,
    -0.0026878227142765033f,
    -0.0042642298786171248f,
    -0.0051544660975044896f,
    -0.0012748362894815883f,
    0.0033275424485822258f,
    0.0087987738495918962f,
    -0.00029146472937361539f,
    0.012289543775634562f,
    0.0083818827597502768f
};

const float spectrumCyanL[SPECTRUM_SAMPLES] =
{
    1.1348398575844034f,
    1.1356676971486841f,
    1.1357070199851398f,
    1.1360466055598115f,
    1.136201845932568f,
    1.1363868454559742f,
    1.1359392061898217f,
    1.135905732352664f,
    1.1362765526597087f,
    1.1360422443251623f,
    1.1359209622888833f,
    1.1355850339063351f,
    1.1360992588687713f,
    1.1357579787451224f,
    1.1354384293956774f,
    1.1351614338712421f,
    1.1257888667495031f,
    1.0536124582564839f,
    0.85831688618440871f,
    0.60020457860571697f,
    0.33153538161715967f,
    0.13989333293667652f,
    0.024000359810648023f,
    -0.011926775546481416f,
    -0.011704986295282091f,
    -0.011437483412541681f,
    -0.010322596803388097f,
    -0.0060091685578381963f,
    -0.0077700895917891888f,
    -0.0089901412571611241f,
    -0.0066276163675329774f
};

const float spectrumMagenta[SPECTRUM_SAMPLES] =
{
    0.98682610550264005f,
    1.0002554017757699f,
    1.0172178417101179f,
    1.0175415023414365f,
    1.0199955188301053f,
    1.005095411447277f,
    1.003926628129244f,
    1.0149673280857643f,
    0.89512349494577603f,
    0.50095445912283054f,
    0.048164444768079351f,
    0.0056723747477411997f,
    0.0034201165376740025f,
    0.00064013646992470887f,
    -0.0055532696085100483f,
    -0.0028326747757337341f,
    0.00088747378997870827f,
    0.060611032523031967f,
    0.29127935939405758f,
    0.63517937541464253f,
    0.92008255788281146f,
    0.97416042025684735f,
    1.0022268450290279f,
    1.0069683834077647f,
    0.97592627805460264f,
    0.91612831480616896f,
    0.8784198033423547f,
    0.9419434628885871f,
    0.95235973844410782f,
    0.97996641063015655f,
    0.90105813742120378f
};

const float spectrumMagentaL[SPECTRUM_SAMPLES] =
{
    1.0762810768395699f,
    1.0769319259849968f,
    1.0785677797287292f,
    1.0750674202655903f,
    1.0731140937136301f,
    1.073136799672074f,
    1.0783475449734545f,
    1.0831165891482393f,
    1.0416328023039758f,
    0.86421707435025119f,
    0.50620815720090007f,
    0.13481366990127519f,
    0.016461225818586996f,
    -0.0016110734727325571f,
    -0.001577562125243675f,
    -0.0005623974861338521f,
    -0.00010701289938895059f,
    0.0038939126486670062f,
    0.061661270831808682f,
    0.26927117954391622f,
    0.51302943019069713f,
    0.82437800070808942f,
    1.0163971798254958f,
    1.080052789131702f,
    1.0777155206011226f,
    1.0495152266336965f,
    1.022980934765086f,
    1.0340676248885061f,
    1.0626466438369728f,
    1.0062423356499561f,
    1.0459250734905665f
};

const float spectrumYellow[SPECTRUM_SAMPLES] =
{
    -0.0055854295800273008f,
    -0.0063025069321834496f,
    -0.0055411533976823081f,
    -0.0014462402814954163f,
    0.014769931784255612f,
    0.070723823947430806f,
    0.16096296004144861f,
    0.27813718052166814f,
    0.41947590321952938f,
    0.58254157729665901f,
    0.75792597532220995f,
    0.91166482602676457f,
    1.0219919351330606f,
    1.0498681056967727f,
    1.0512693458031506f,
    1.0511261354875177f,
    1.0513172665101362f,
    1.0516893715834366f,
    1.051530568007089f,
    1.0512293439748015f,
    1.0512001044606694f,
    1.0515340253157541f,
    1.0514906332285603f,
    1.0515101742027935f,
    1.051396416405121f,
    1.0512709587055149f,
    1.0513525294327393f,
    1.0509320938066786f,
    1.0505896725344117f,
    1.0484428591209363f,
    1.0487805509613644f
};

const float spectrumYellowL[SPECTRUM_SAMPLES] =
{
    6.9971359282283433e-05f,
    0.00021547621500355132f,
    -0.00017249752328586397f,
    -9.6888942433033859e-05f,
    -0.00019307056462793788f,
    0.00025716375046497166f,
    0.027733638661711475f,
    0.24180885231565283f,
    0.6621473311856314f,
    1.013382188677638f,
    1.0336106474747151f,
    1.0363414810285585f,
    1.0365541783165932f,
    1.0365330262601604f,
    1.036737233491474f,
    1.0367120379646697f,
    1.0364995083975292f,
    1.0364890940418487f,
    1.036665987124938f,
    1.036496286083382f,
    1.0361220276547307f,
    1.0342205252338599f,
    1.0131423004390736f,
    0.91368988283435559f,
    0.79603870191606219f,
    0.71221251530299901f,
    0.64838441348465592f,
    0.60751269922295625f,
    0.59660447413700568f,
    0.59302577055330419f,
    0.573287901892368f
};

const float spectrumRed[SPECTRUM_SAMPLES] =
{
    0.12105572482940778f,
    0.10741317347334768f,
    0.076457032778233097f,
    0.03660090047276178f,
    0.00030965866560745406f,
    0.0083920547349397075f,
    0.010948634822196657f,
    0.0090087426074995352f,
    0.0070910626387294534f,
    -0.0043655732716718666f,
    -0.0035208146618901509f,
    -0.0087431774834964785f,
    -0.0048194282717736068f,
    -0.0085243507904226388f,
    -0.0090749252993921579f,
    -0.0051446257036521668f,
    0.0012909938990138092f,
    0.002571892584388627f,
    0.11335845554652371f,
    0.59632435225633906f,
    0.92826939418992138f,
    0.99290238673769138f,
    1.0005238261425027f,
    0.99745743168305834f,
    0.99539828241702666f,
    1.0022589001957172f,
    1.0022703654576719f,
    0.9892971313424096f,
    1.00210930696114f,
    0.9817108601420349f,
    0.98145822341144662f
};

const float spectrumRedL[SPECTRUM_SAMPLES] =
{
    0.059387987189333913f,
    0.054481524357465283f,
    0.046113141944363924f,
    0.03834916364811549f,
    0.027004243435414778f,
    0.010665103970216742f,
    0.0014064658931301416f,
    0.00011230632364564261f,
    0.00075695649887125532f,
    0.00020048527860242049f,
    -0.00032268079438516306f,
    -0.0001391022345515706f,
    -0.00011872779221617178f,
    -0.00014009438689342228f,
    -0.00017983726095631365f,
    -0.00034993389174970497f,
    0.010167902964022995f,
    0.064438031786600763f,
    0.19139615608454164f,
    0.36041326035179472f,
    0.55161702610768149f,
    0.72276498624120544f,
    0.84975843023643782f,
    0.93196215850464803f,
    0.97490943111565131f,
    0.99197346495671401f,
    0.99558882524000814f,
    0.98843651314221781f,
    0.99214437001890787f,
    0.97928394916155659f,
    0.98649440746380113f
};

const float spectrumGreen[SPECTRUM_SAMPLES] =
{
    -0.011541397045043738f,
    -0.010209128358026206f,
    -0.011652682845286444f,
    -0.0086546327246821552f,
    -0.0080196261480055785f,
    -0.0083183001054456009f,
    0.032457730677471958f,
    0.21117137586599877f,
    0.51134258036396607f,
    0.79138683996339754f,
    0.95437146800026929f,
    0.99431293608007743f,
    0.99963170059787598f,
    0.99955278360457456f,
    0.9997301722851818f,
    0.99966746579308785f,
    0.99928513434408917f,
    0.98812868234570472f,
    0.89772082051302782f,
    0.61648283446599983f,
    0.29980353327788162f,
    0.067980829090419137f,
    0.0010445407603993568f,
    -0.0038863693928531686f,
    -0.0055688297192008837f,
    -0.007584579876037177f,
    -0.008862724982289908f,
    -0.008587382851897354f,
    -0.0083488793391171909f,
    -0.007581291470542771f,
    -0.0021314239379536626f
};

const float spectrumGreenL[SPECTRUM_SAMPLES] =
{
    0.007048067794274554f,
    0.0057158093785145726f,
    0.00085270631249813116f,
    -0.0014917696037937082f,
    -0.015322899704203325f,
    0.0032112962393825886f,
    0.0084791373027953594f,
    0.14492486297187904f,
    0.5515792750878783f,
    0.99870031202229792f,
    1.031660757508003f,
    1.0326139669290242f,
    1.0341228383337173f,
    1.0213101303084535f,
    1.0266337377586581f,
    1.0354724811898972f,
    1.0367465662770763f,
    1.0324407159588025f,
    1.013061979274551f,
    0.85200706335417065f,
    0.14999291197428011f,
    -0.00020522094102810778f,
    0.0047805768225471822f,
    0.0033550693193379381f,
    0.0099399772088486788f,
    0.015978761112753165f,
    0.0047878540766865619f,
    0.0015115125916503433f,
    -0.00031059862279848497f,
    -0.0043442567037483924f,
    0.0060996163684375404f
};

const float spectrumBlue[SPECTRUM_SAMPLES] =
{
    0.99524602093178505f,
    0.99461584272465509f,
    0.99320833125848673f,
    0.99887681621820679f,
    0.99982704093455643f,
    0.9996997187786808f,
    0.99158766902939299f,
    0.89912483784599662f,
    0.71578384482310409f,
    0.51357444461982482f,
    0.31665694269346367f,
    0.14998271536756969f,
    0.041126433800721895f,
    0.0053295675757349939f,
    -0.00026298148656458238f,
    -0.00042165039289576343f,
    0.00041333969651849314f,
    0.0022886564055473214f,
    0.0028461380038644801f,
    -6.9353185581720454e-05f,
    0.0004144530438565336f,
    0.006348394598516066f,
    0.019859428386052359f,
    0.032710159833497544f,
    0.043163364746243972f,
    0.049436102460510223f,
    0.049644578435863541f,
    0.048215061394125756f,
    0.040194797603106215f,
    0.031878165297337613f,
    0.023604557408827033f
};

const float spectrumBlueL[SPECTRUM_SAMPLES] =
{
    1.0544610530770391f,
    1.0541102739650707f,
    1.0574985392685119f,
    1.0579007579297524f,
    1.0581911697421313f,
    1.0580706223158576f,
    1.0569749852191395f,
    1.0566082043642206f,
    1.0523075558997743f,
    0.87303211227288036f,
    0.32452402839240918f,
    0.039290429443487825f,
    -0.0014262344212532885f,
    -0.0013937997222664299f,
    -0.00135115759792127f,
    -0.0016095441459695152f,
    -0.0006974770074028334f,
    0.00039507350339585328f,
    -0.0014086658278202266f,
    -0.0013906360550582043f,
    -0.0016553172871414835f,
    -0.00099684043493759129f,
    0.0066544498386996069f,
    0.02203462959158527f,
    0.056649179049162571f,
    0.10488076535885001f,
    0.14111150525084881f,
    0.1531351631479525f,
    0.15128623413730849f,
    0.16300301123579944f,
    0.16870528814861985f
};

const float spectrumOne[SPECTRUM_SAMPLES] =
{
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f
};

#endif

//More memory usage but I avoid to construct and destroy these multiple times
//while parsing RGB values
const Spectrum SPECTRUM_WHITE(spectrumWhite);
const Spectrum SPECTRUM_WHITEL(spectrumWhiteL);
const Spectrum SPECTRUM_CYAN(spectrumCyan);
const Spectrum SPECTRUM_CYANL(spectrumCyanL);
const Spectrum SPECTRUM_MAGENTA(spectrumMagenta);
const Spectrum SPECTRUM_MAGENTAL(spectrumMagentaL);
const Spectrum SPECTRUM_YELLOW(spectrumYellow);
const Spectrum SPECTRUM_YELLOWL(spectrumYellowL);
const Spectrum SPECTRUM_RED(spectrumRed);
const Spectrum SPECTRUM_REDL(spectrumRedL);
const Spectrum SPECTRUM_GREEN(spectrumGreen);
const Spectrum SPECTRUM_GREENL(spectrumGreenL);
const Spectrum SPECTRUM_BLUE(spectrumBlue);
const Spectrum SPECTRUM_BLUEL(spectrumBlueL);
const Spectrum SPECTRUM_ONE(spectrumOne);
const Spectrum SPECTRUM_BLACK(0);

Spectrum::Spectrum()
{
    //do nothing. Too expensive to initialize if I need to assign it later
}

Spectrum::Spectrum(int t)
{
    if(t==0) //black
        memset(Spectrum::w,0,sizeof(float)*SPECTRUM_SAMPLES);
    else //apply planck's law
    {
        //TODO: this must be changed if not vacuum
        constexpr const float c = 299792458;
        constexpr const float c2 = c*c;
        ///first radiation constant: 2hc^2
        constexpr float frc = 2.f*PLANCK_H*c2;
        //second radiation constant: hc/kb
        constexpr float src = PLANCK_H*c/BOLTZMANN_K;
        float current_wavelength = SPECTRUM_START*1E-9f;
        for(int i=0;i<SPECTRUM_SAMPLES;i++)
        {
            Spectrum::w[i]=frc/powf(current_wavelength,5);
            w[i]*=1.f/(expf(src/(t*current_wavelength))+1.f);
            current_wavelength+=SPECTRUM_INTERVAL*1E-9f;
        }
    }
}

Spectrum::Spectrum(const float* vals)
{
    for(int i=0;i<SPECTRUM_SAMPLES;i++)
        Spectrum::w[i] = vals[i];
}

Spectrum::Spectrum(float val)
{
    Spectrum::w[0] = val;
    Spectrum::w[1] = val;
    Spectrum::w[2] = val;
    Spectrum::w[3] = val;
    Spectrum::w[4] = val;
    Spectrum::w[5] = val;
    Spectrum::w[6] = val;
    Spectrum::w[7] = val;
    Spectrum::w[8] = val;
    Spectrum::w[9] = val;
    Spectrum::w[10] = val;
    Spectrum::w[11] = val;
    Spectrum::w[12] = val;
    Spectrum::w[13] = val;
    Spectrum::w[14] = val;
    Spectrum::w[15] = val;
#if HQ_SPECTRUM!=0
    Spectrum::w[16] = val;
    Spectrum::w[17] = val;
    Spectrum::w[18] = val;
    Spectrum::w[19] = val;
    Spectrum::w[20] = val;
    Spectrum::w[21] = val;
    Spectrum::w[22] = val;
    Spectrum::w[23] = val;
    Spectrum::w[24] = val;
    Spectrum::w[25] = val;
    Spectrum::w[26] = val;
    Spectrum::w[27] = val;
    Spectrum::w[28] = val;
    Spectrum::w[29] = val;
    Spectrum::w[30] = val;
#endif

}

Spectrum::Spectrum(ColorRGB c, bool l)
{
    memset(Spectrum::w,0,sizeof(float)*SPECTRUM_SAMPLES);
    if(!l)
    {
        if(c.r<=c.g && c.r<=c.b)
        {
            *this += SPECTRUM_WHITE*c.r;
            if(c.g <= c.b)
            {
                *this += SPECTRUM_CYAN*(c.g-c.r);
                *this += SPECTRUM_BLUE*(c.b-c.g);
            }
            else
            {
                *this += SPECTRUM_CYAN*(c.b-c.r);
                *this += SPECTRUM_GREEN*(c.g-c.b);
            }
        }
        else if(c.g<=c.r && c.g<=c.b)
        {
            *this += SPECTRUM_WHITE*c.g;
            if(c.r <= c.b)
            {
                *this += SPECTRUM_MAGENTA*(c.r-c.g);
                *this += SPECTRUM_BLUE*(c.b-c.r);
            }
            else
            {
                *this += SPECTRUM_MAGENTA*(c.b-c.g);
                *this += SPECTRUM_RED*(c.r-c.b);
            }
        }
        else
        {
            *this += SPECTRUM_WHITE*c.b;
            if(c.r <= c.g)
            {
                *this += SPECTRUM_YELLOW*(c.r-c.b);
                *this += SPECTRUM_GREEN*(c.g-c.r);
            }
            else
            {
                *this += SPECTRUM_YELLOW*(c.g-c.b);
                *this += SPECTRUM_RED*(c.r-c.g);
            }
        }
        *this *= 0.94f;
    }
    else
    {
        if(c.r<=c.g && c.r<=c.b)
        {
            *this += SPECTRUM_WHITEL*c.r;
            if(c.g <= c.b)
            {
                *this += SPECTRUM_CYANL*(c.g-c.r);
                *this += SPECTRUM_BLUEL*(c.b-c.g);
            }
            else
            {
                *this += SPECTRUM_CYANL*(c.b-c.r);
                *this += SPECTRUM_GREENL*(c.g-c.b);
            }
        }
        else if(c.g<=c.r && c.g<=c.b)
        {
            *this += SPECTRUM_WHITEL*c.g;
            if(c.r <= c.b)
            {
                *this += SPECTRUM_MAGENTAL*(c.r-c.g);
                *this += SPECTRUM_BLUEL*(c.b-c.r);
            }
            else
            {
                *this += SPECTRUM_MAGENTAL*(c.b-c.g);
                *this += SPECTRUM_REDL*(c.r-c.b);
            }
        }
        else
        {
            *this += SPECTRUM_WHITEL*c.b;
            if(c.r <= c.g)
            {
                *this += SPECTRUM_YELLOWL*(c.r-c.b);
                *this += SPECTRUM_GREENL*(c.g-c.r);
            }
            else
            {
                *this += SPECTRUM_YELLOWL*(c.g-c.b);
                *this += SPECTRUM_REDL*(c.r-c.g);
            }
        }
        *this *= 0.86445f;
    }
}

ColorXYZ Spectrum::toXYZ()const
{
    float x = 0;
    float y = 0;
    float z = 0;
    x+=Spectrum::w[0]*X[0];
    x+=Spectrum::w[1]*X[1];
    x+=Spectrum::w[2]*X[2];
    x+=Spectrum::w[3]*X[3];
    x+=Spectrum::w[4]*X[4];
    x+=Spectrum::w[5]*X[5];
    x+=Spectrum::w[6]*X[6];
    x+=Spectrum::w[7]*X[7];
    x+=Spectrum::w[8]*X[8];
    x+=Spectrum::w[9]*X[9];
    x+=Spectrum::w[10]*X[10];
    x+=Spectrum::w[11]*X[11];
    x+=Spectrum::w[12]*X[12];
    x+=Spectrum::w[13]*X[13];
    x+=Spectrum::w[14]*X[14];
    x+=Spectrum::w[15]*X[15];
    
    y+=Spectrum::w[0]*Y[0];
    y+=Spectrum::w[1]*Y[1];
    y+=Spectrum::w[2]*Y[2];
    y+=Spectrum::w[3]*Y[3];
    y+=Spectrum::w[4]*Y[4];
    y+=Spectrum::w[5]*Y[5];
    y+=Spectrum::w[6]*Y[6];
    y+=Spectrum::w[7]*Y[7];
    y+=Spectrum::w[8]*Y[8];
    y+=Spectrum::w[9]*Y[9];
    y+=Spectrum::w[10]*Y[10];
    y+=Spectrum::w[11]*Y[11];
    y+=Spectrum::w[12]*Y[12];
    y+=Spectrum::w[13]*Y[13];
    y+=Spectrum::w[14]*Y[14];
    y+=Spectrum::w[15]*Y[15];
    
    z+=Spectrum::w[0]*Z[0];
    z+=Spectrum::w[1]*Z[1];
    z+=Spectrum::w[2]*Z[2];
    z+=Spectrum::w[3]*Z[3];
    z+=Spectrum::w[4]*Z[4];
    z+=Spectrum::w[5]*Z[5];
    z+=Spectrum::w[6]*Z[6];
    z+=Spectrum::w[7]*Z[7];
    z+=Spectrum::w[8]*Z[8];
    z+=Spectrum::w[9]*Z[9];
    z+=Spectrum::w[10]*Z[10];
    z+=Spectrum::w[11]*Z[11];
    z+=Spectrum::w[12]*Z[12];
    z+=Spectrum::w[13]*Z[13];
    z+=Spectrum::w[14]*Z[14];
    z+=Spectrum::w[15]*Z[15];
#if HQ_SPECTRUM!=0
    x+=Spectrum::w[16]*X[16];
    x+=Spectrum::w[17]*X[17];
    x+=Spectrum::w[18]*X[18];
    x+=Spectrum::w[19]*X[19];
    x+=Spectrum::w[20]*X[20];
    x+=Spectrum::w[21]*X[21];
    x+=Spectrum::w[22]*X[22];
    x+=Spectrum::w[23]*X[23];
    x+=Spectrum::w[24]*X[24];
    x+=Spectrum::w[25]*X[25];
    x+=Spectrum::w[26]*X[26];
    x+=Spectrum::w[27]*X[27];
    x+=Spectrum::w[28]*X[28];
    x+=Spectrum::w[29]*X[29];
    x+=Spectrum::w[30]*X[30];
    
    y+=Spectrum::w[16]*Y[16];
    y+=Spectrum::w[17]*Y[17];
    y+=Spectrum::w[18]*Y[18];
    y+=Spectrum::w[19]*Y[19];
    y+=Spectrum::w[20]*Y[20];
    y+=Spectrum::w[21]*Y[21];
    y+=Spectrum::w[22]*Y[22];
    y+=Spectrum::w[23]*Y[23];
    y+=Spectrum::w[24]*Y[24];
    y+=Spectrum::w[25]*Y[25];
    y+=Spectrum::w[26]*Y[26];
    y+=Spectrum::w[27]*Y[27];
    y+=Spectrum::w[28]*Y[28];
    y+=Spectrum::w[29]*Y[29];
    y+=Spectrum::w[30]*Y[30];
    
    z+=Spectrum::w[16]*Z[16];
    z+=Spectrum::w[17]*Z[17];
    z+=Spectrum::w[18]*Z[18];
    z+=Spectrum::w[19]*Z[19];
    z+=Spectrum::w[20]*Z[20];
    z+=Spectrum::w[21]*Z[21];
    z+=Spectrum::w[22]*Z[22];
    z+=Spectrum::w[23]*Z[23];
    z+=Spectrum::w[24]*Z[24];
    z+=Spectrum::w[25]*Z[25];
    z+=Spectrum::w[26]*Z[26];
    z+=Spectrum::w[27]*Z[27];
    z+=Spectrum::w[28]*Z[28];
    z+=Spectrum::w[29]*Z[29];
    z+=Spectrum::w[30]*Z[30];
#endif
    x*=INVY_SUM;
    y*=INVY_SUM;
    z*=INVY_SUM;
    return ColorXYZ(x,y,z);
}

float Spectrum::luminance()const
{
    float y = 0;
    y+=Spectrum::w[0]*Y[0];
    y+=Spectrum::w[1]*Y[1];
    y+=Spectrum::w[2]*Y[2];
    y+=Spectrum::w[3]*Y[3];
    y+=Spectrum::w[4]*Y[4];
    y+=Spectrum::w[5]*Y[5];
    y+=Spectrum::w[6]*Y[6];
    y+=Spectrum::w[7]*Y[7];
    y+=Spectrum::w[8]*Y[8];
    y+=Spectrum::w[9]*Y[9];
    y+=Spectrum::w[10]*Y[10];
    y+=Spectrum::w[11]*Y[11];
    y+=Spectrum::w[12]*Y[12];
    y+=Spectrum::w[13]*Y[13];
    y+=Spectrum::w[14]*Y[14];
    y+=Spectrum::w[15]*Y[15];
#if HQ_SPECTRUM!=0
    y+=Spectrum::w[16]*Y[16];
    y+=Spectrum::w[17]*Y[17];
    y+=Spectrum::w[18]*Y[18];
    y+=Spectrum::w[19]*Y[19];
    y+=Spectrum::w[20]*Y[20];
    y+=Spectrum::w[21]*Y[21];
    y+=Spectrum::w[22]*Y[22];
    y+=Spectrum::w[23]*Y[23];
    y+=Spectrum::w[24]*Y[24];
    y+=Spectrum::w[25]*Y[25];
    y+=Spectrum::w[26]*Y[26];
    y+=Spectrum::w[27]*Y[27];
    y+=Spectrum::w[28]*Y[28];
    y+=Spectrum::w[29]*Y[29];
    y+=Spectrum::w[30]*Y[30];
#endif
    
    return y*INVY_SUM;
}

bool Spectrum::isBlack()const
{
    for(int i=0;i<SPECTRUM_SAMPLES;i++)
        if(Spectrum::w[i]!=0)
            return false;
    return true;
}

StratifiedSpectrum Spectrum::stratify(float r0, float* pdf)const
{
    unsigned char chosen;
    chosen = (unsigned char)min((int)(r0*SPECTRUM_SAMPLES),SPECTRUM_SAMPLES-1);
    *pdf/=SPECTRUM_SAMPLES;
    return StratifiedSpectrum(Spectrum::w[chosen],chosen);
}

Spectrum Spectrum::weight()const
{
    return SPECTRUM_ONE;
}


Spectrum Spectrum::operator+(const Spectrum& s)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] + s.w[0];
    retval.w[1] = Spectrum::w[1] + s.w[1];
    retval.w[2] = Spectrum::w[2] + s.w[2];
    retval.w[3] = Spectrum::w[3] + s.w[3];
    retval.w[4] = Spectrum::w[4] + s.w[4];
    retval.w[5] = Spectrum::w[5] + s.w[5];
    retval.w[6] = Spectrum::w[6] + s.w[6];
    retval.w[7] = Spectrum::w[7] + s.w[7];
    retval.w[8] = Spectrum::w[8] + s.w[8];
    retval.w[9] = Spectrum::w[9] + s.w[9];
    retval.w[10] = Spectrum::w[10] + s.w[10];
    retval.w[11] = Spectrum::w[11] + s.w[11];
    retval.w[12] = Spectrum::w[12] + s.w[12];
    retval.w[13] = Spectrum::w[13] + s.w[13];
    retval.w[14] = Spectrum::w[14] + s.w[14];
    retval.w[15] = Spectrum::w[15] + s.w[15];
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] + s.w[16];
    retval.w[17] = Spectrum::w[17] + s.w[17];
    retval.w[18] = Spectrum::w[18] + s.w[18];
    retval.w[19] = Spectrum::w[19] + s.w[19];
    retval.w[20] = Spectrum::w[20] + s.w[20];
    retval.w[21] = Spectrum::w[21] + s.w[21];
    retval.w[22] = Spectrum::w[22] + s.w[22];
    retval.w[23] = Spectrum::w[23] + s.w[23];
    retval.w[24] = Spectrum::w[24] + s.w[24];
    retval.w[25] = Spectrum::w[25] + s.w[25];
    retval.w[26] = Spectrum::w[26] + s.w[26];
    retval.w[27] = Spectrum::w[27] + s.w[27];
    retval.w[28] = Spectrum::w[28] + s.w[28];
    retval.w[29] = Spectrum::w[29] + s.w[29];
    retval.w[30] = Spectrum::w[30] + s.w[30];
#endif
    return retval;
}

void Spectrum::operator+=(const Spectrum& s)
{
    Spectrum::w[0] += s.w[0];
    Spectrum::w[1] += s.w[1];
    Spectrum::w[2] += s.w[2];
    Spectrum::w[3] += s.w[3];
    Spectrum::w[4] += s.w[4];
    Spectrum::w[5] += s.w[5];
    Spectrum::w[6] += s.w[6];
    Spectrum::w[7] += s.w[7];
    Spectrum::w[8] += s.w[8];
    Spectrum::w[9] += s.w[9];
    Spectrum::w[10] += s.w[10];
    Spectrum::w[11] += s.w[11];
    Spectrum::w[12] += s.w[12];
    Spectrum::w[13] += s.w[13];
    Spectrum::w[14] += s.w[14];
    Spectrum::w[15] += s.w[15];
#if HQ_SPECTRUM!=0
    Spectrum::w[16] += s.w[16];
    Spectrum::w[17] += s.w[17];
    Spectrum::w[18] += s.w[18];
    Spectrum::w[19] += s.w[19];
    Spectrum::w[20] += s.w[20];
    Spectrum::w[21] += s.w[21];
    Spectrum::w[22] += s.w[22];
    Spectrum::w[23] += s.w[23];
    Spectrum::w[24] += s.w[24];
    Spectrum::w[25] += s.w[25];
    Spectrum::w[26] += s.w[26];
    Spectrum::w[27] += s.w[27];
    Spectrum::w[28] += s.w[28];
    Spectrum::w[29] += s.w[29];
    Spectrum::w[30] += s.w[30];
#endif
}

Spectrum Spectrum::operator-(const Spectrum& s)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] - s.w[0];
    retval.w[1] = Spectrum::w[1] - s.w[1];
    retval.w[2] = Spectrum::w[2] - s.w[2];
    retval.w[3] = Spectrum::w[3] - s.w[3];
    retval.w[4] = Spectrum::w[4] - s.w[4];
    retval.w[5] = Spectrum::w[5] - s.w[5];
    retval.w[6] = Spectrum::w[6] - s.w[6];
    retval.w[7] = Spectrum::w[7] - s.w[7];
    retval.w[8] = Spectrum::w[8] - s.w[8];
    retval.w[9] = Spectrum::w[9] - s.w[9];
    retval.w[10] = Spectrum::w[10] - s.w[10];
    retval.w[11] = Spectrum::w[11] - s.w[11];
    retval.w[12] = Spectrum::w[12] - s.w[12];
    retval.w[13] = Spectrum::w[13] - s.w[13];
    retval.w[14] = Spectrum::w[14] - s.w[14];
    retval.w[15] = Spectrum::w[15] - s.w[15];
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] - s.w[16];
    retval.w[17] = Spectrum::w[17] - s.w[17];
    retval.w[18] = Spectrum::w[18] - s.w[18];
    retval.w[19] = Spectrum::w[19] - s.w[19];
    retval.w[20] = Spectrum::w[20] - s.w[20];
    retval.w[21] = Spectrum::w[21] - s.w[21];
    retval.w[22] = Spectrum::w[22] - s.w[22];
    retval.w[23] = Spectrum::w[23] - s.w[23];
    retval.w[24] = Spectrum::w[24] - s.w[24];
    retval.w[25] = Spectrum::w[25] - s.w[25];
    retval.w[26] = Spectrum::w[26] - s.w[26];
    retval.w[27] = Spectrum::w[27] - s.w[27];
    retval.w[28] = Spectrum::w[28] - s.w[28];
    retval.w[29] = Spectrum::w[29] - s.w[29];
    retval.w[30] = Spectrum::w[30] - s.w[30];
#endif
    return retval;

}

void Spectrum::operator-=(const Spectrum& s)
{
    Spectrum::w[0] -= s.w[0];
    Spectrum::w[1] -= s.w[1];
    Spectrum::w[2] -= s.w[2];
    Spectrum::w[3] -= s.w[3];
    Spectrum::w[4] -= s.w[4];
    Spectrum::w[5] -= s.w[5];
    Spectrum::w[6] -= s.w[6];
    Spectrum::w[7] -= s.w[7];
    Spectrum::w[8] -= s.w[8];
    Spectrum::w[9] -= s.w[9];
    Spectrum::w[10] -= s.w[10];
    Spectrum::w[11] -= s.w[11];
    Spectrum::w[12] -= s.w[12];
    Spectrum::w[13] -= s.w[13];
    Spectrum::w[14] -= s.w[14];
    Spectrum::w[15] -= s.w[15];
#if HQ_SPECTRUM!=0
    Spectrum::w[16] -= s.w[16];
    Spectrum::w[17] -= s.w[17];
    Spectrum::w[18] -= s.w[18];
    Spectrum::w[19] -= s.w[19];
    Spectrum::w[20] -= s.w[20];
    Spectrum::w[21] -= s.w[21];
    Spectrum::w[22] -= s.w[22];
    Spectrum::w[23] -= s.w[23];
    Spectrum::w[24] -= s.w[24];
    Spectrum::w[25] -= s.w[25];
    Spectrum::w[26] -= s.w[26];
    Spectrum::w[27] -= s.w[27];
    Spectrum::w[28] -= s.w[28];
    Spectrum::w[29] -= s.w[29];
    Spectrum::w[30] -= s.w[30];
#endif
}

Spectrum Spectrum::operator*(const Spectrum& s)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] * s.w[0];
    retval.w[1] = Spectrum::w[1] * s.w[1];
    retval.w[2] = Spectrum::w[2] * s.w[2];
    retval.w[3] = Spectrum::w[3] * s.w[3];
    retval.w[4] = Spectrum::w[4] * s.w[4];
    retval.w[5] = Spectrum::w[5] * s.w[5];
    retval.w[6] = Spectrum::w[6] * s.w[6];
    retval.w[7] = Spectrum::w[7] * s.w[7];
    retval.w[8] = Spectrum::w[8] * s.w[8];
    retval.w[9] = Spectrum::w[9] * s.w[9];
    retval.w[10] = Spectrum::w[10] * s.w[10];
    retval.w[11] = Spectrum::w[11] * s.w[11];
    retval.w[12] = Spectrum::w[12] * s.w[12];
    retval.w[13] = Spectrum::w[13] * s.w[13];
    retval.w[14] = Spectrum::w[14] * s.w[14];
    retval.w[15] = Spectrum::w[15] * s.w[15];
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] * s.w[16];
    retval.w[17] = Spectrum::w[17] * s.w[17];
    retval.w[18] = Spectrum::w[18] * s.w[18];
    retval.w[19] = Spectrum::w[19] * s.w[19];
    retval.w[20] = Spectrum::w[20] * s.w[20];
    retval.w[21] = Spectrum::w[21] * s.w[21];
    retval.w[22] = Spectrum::w[22] * s.w[22];
    retval.w[23] = Spectrum::w[23] * s.w[23];
    retval.w[24] = Spectrum::w[24] * s.w[24];
    retval.w[25] = Spectrum::w[25] * s.w[25];
    retval.w[26] = Spectrum::w[26] * s.w[26];
    retval.w[27] = Spectrum::w[27] * s.w[27];
    retval.w[28] = Spectrum::w[28] * s.w[28];
    retval.w[29] = Spectrum::w[29] * s.w[29];
    retval.w[30] = Spectrum::w[30] * s.w[30];
#endif
    return retval;

}

void Spectrum::operator*=(const Spectrum& s)
{
    Spectrum::w[0] *= s.w[0];
    Spectrum::w[1] *= s.w[1];
    Spectrum::w[2] *= s.w[2];
    Spectrum::w[3] *= s.w[3];
    Spectrum::w[4] *= s.w[4];
    Spectrum::w[5] *= s.w[5];
    Spectrum::w[6] *= s.w[6];
    Spectrum::w[7] *= s.w[7];
    Spectrum::w[8] *= s.w[8];
    Spectrum::w[9] *= s.w[9];
    Spectrum::w[10] *= s.w[10];
    Spectrum::w[11] *= s.w[11];
    Spectrum::w[12] *= s.w[12];
    Spectrum::w[13] *= s.w[13];
    Spectrum::w[14] *= s.w[14];
    Spectrum::w[15] *= s.w[15];
#if HQ_SPECTRUM!=0
    Spectrum::w[16] *= s.w[16];
    Spectrum::w[17] *= s.w[17];
    Spectrum::w[18] *= s.w[18];
    Spectrum::w[19] *= s.w[19];
    Spectrum::w[20] *= s.w[20];
    Spectrum::w[21] *= s.w[21];
    Spectrum::w[22] *= s.w[22];
    Spectrum::w[23] *= s.w[23];
    Spectrum::w[24] *= s.w[24];
    Spectrum::w[25] *= s.w[25];
    Spectrum::w[26] *= s.w[26];
    Spectrum::w[27] *= s.w[27];
    Spectrum::w[28] *= s.w[28];
    Spectrum::w[29] *= s.w[29];
    Spectrum::w[30] *= s.w[30];
#endif
}

Spectrum Spectrum::operator/(const Spectrum& s)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] / s.w[0];
    retval.w[1] = Spectrum::w[1] / s.w[1];
    retval.w[2] = Spectrum::w[2] / s.w[2];
    retval.w[3] = Spectrum::w[3] / s.w[3];
    retval.w[4] = Spectrum::w[4] / s.w[4];
    retval.w[5] = Spectrum::w[5] / s.w[5];
    retval.w[6] = Spectrum::w[6] / s.w[6];
    retval.w[7] = Spectrum::w[7] / s.w[7];
    retval.w[8] = Spectrum::w[8] / s.w[8];
    retval.w[9] = Spectrum::w[9] / s.w[9];
    retval.w[10] = Spectrum::w[10] / s.w[10];
    retval.w[11] = Spectrum::w[11] / s.w[11];
    retval.w[12] = Spectrum::w[12] / s.w[12];
    retval.w[13] = Spectrum::w[13] / s.w[13];
    retval.w[14] = Spectrum::w[14] / s.w[14];
    retval.w[15] = Spectrum::w[15] / s.w[15];
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] / s.w[16];
    retval.w[17] = Spectrum::w[17] / s.w[17];
    retval.w[18] = Spectrum::w[18] / s.w[18];
    retval.w[19] = Spectrum::w[19] / s.w[19];
    retval.w[20] = Spectrum::w[20] / s.w[20];
    retval.w[21] = Spectrum::w[21] / s.w[21];
    retval.w[22] = Spectrum::w[22] / s.w[22];
    retval.w[23] = Spectrum::w[23] / s.w[23];
    retval.w[24] = Spectrum::w[24] / s.w[24];
    retval.w[25] = Spectrum::w[25] / s.w[25];
    retval.w[26] = Spectrum::w[26] / s.w[26];
    retval.w[27] = Spectrum::w[27] / s.w[27];
    retval.w[28] = Spectrum::w[28] / s.w[28];
    retval.w[29] = Spectrum::w[29] / s.w[29];
    retval.w[30] = Spectrum::w[30] / s.w[30];
#endif
    return retval;
}

void Spectrum::operator/=(const Spectrum& s)
{
    Spectrum::w[0] /= s.w[0];
    Spectrum::w[1] /= s.w[1];
    Spectrum::w[2] /= s.w[2];
    Spectrum::w[3] /= s.w[3];
    Spectrum::w[4] /= s.w[4];
    Spectrum::w[5] /= s.w[5];
    Spectrum::w[6] /= s.w[6];
    Spectrum::w[7] /= s.w[7];
    Spectrum::w[8] /= s.w[8];
    Spectrum::w[9] /= s.w[9];
    Spectrum::w[10] /= s.w[10];
    Spectrum::w[11] /= s.w[11];
    Spectrum::w[12] /= s.w[12];
    Spectrum::w[13] /= s.w[13];
    Spectrum::w[14] /= s.w[14];
    Spectrum::w[15] /= s.w[15];
#if HQ_SPECTRUM!=0
    Spectrum::w[16] /= s.w[16];
    Spectrum::w[17] /= s.w[17];
    Spectrum::w[18] /= s.w[18];
    Spectrum::w[19] /= s.w[19];
    Spectrum::w[20] /= s.w[20];
    Spectrum::w[21] /= s.w[21];
    Spectrum::w[22] /= s.w[22];
    Spectrum::w[23] /= s.w[23];
    Spectrum::w[24] /= s.w[24];
    Spectrum::w[25] /= s.w[25];
    Spectrum::w[26] /= s.w[26];
    Spectrum::w[27] /= s.w[27];
    Spectrum::w[28] /= s.w[28];
    Spectrum::w[29] /= s.w[29];
    Spectrum::w[30] /= s.w[30];
#endif
}

Spectrum Spectrum::operator+(float v)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] + v;
    retval.w[1] = Spectrum::w[1] + v;
    retval.w[2] = Spectrum::w[2] + v;
    retval.w[3] = Spectrum::w[3] + v;
    retval.w[4] = Spectrum::w[4] + v;
    retval.w[5] = Spectrum::w[5] + v;
    retval.w[6] = Spectrum::w[6] + v;
    retval.w[7] = Spectrum::w[7] + v;
    retval.w[8] = Spectrum::w[8] + v;
    retval.w[9] = Spectrum::w[9] + v;
    retval.w[10] = Spectrum::w[10] + v;
    retval.w[11] = Spectrum::w[11] + v;
    retval.w[12] = Spectrum::w[12] + v;
    retval.w[13] = Spectrum::w[13] + v;
    retval.w[14] = Spectrum::w[14] + v;
    retval.w[15] = Spectrum::w[15] + v;
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] + v;
    retval.w[17] = Spectrum::w[17] + v;
    retval.w[18] = Spectrum::w[18] + v;
    retval.w[19] = Spectrum::w[19] + v;
    retval.w[20] = Spectrum::w[20] + v;
    retval.w[21] = Spectrum::w[21] + v;
    retval.w[22] = Spectrum::w[22] + v;
    retval.w[23] = Spectrum::w[23] + v;
    retval.w[24] = Spectrum::w[24] + v;
    retval.w[25] = Spectrum::w[25] + v;
    retval.w[26] = Spectrum::w[26] + v;
    retval.w[27] = Spectrum::w[27] + v;
    retval.w[28] = Spectrum::w[28] + v;
    retval.w[29] = Spectrum::w[29] + v;
    retval.w[30] = Spectrum::w[30] + v;
#endif
    return retval;
}

void Spectrum::operator+=(float v)
{
    Spectrum::w[0] += v;
    Spectrum::w[1] += v;
    Spectrum::w[2] += v;
    Spectrum::w[3] += v;
    Spectrum::w[4] += v;
    Spectrum::w[5] += v;
    Spectrum::w[6] += v;
    Spectrum::w[7] += v;
    Spectrum::w[8] += v;
    Spectrum::w[9] += v;
    Spectrum::w[10] += v;
    Spectrum::w[11] += v;
    Spectrum::w[12] += v;
    Spectrum::w[13] += v;
    Spectrum::w[14] += v;
    Spectrum::w[15] += v;
#if HQ_SPECTRUM!=0
    Spectrum::w[16] += v;
    Spectrum::w[17] += v;
    Spectrum::w[18] += v;
    Spectrum::w[19] += v;
    Spectrum::w[20] += v;
    Spectrum::w[21] += v;
    Spectrum::w[22] += v;
    Spectrum::w[23] += v;
    Spectrum::w[24] += v;
    Spectrum::w[25] += v;
    Spectrum::w[26] += v;
    Spectrum::w[27] += v;
    Spectrum::w[28] += v;
    Spectrum::w[29] += v;
    Spectrum::w[30] += v;
#endif
}

Spectrum Spectrum::operator-(float v)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] - v;
    retval.w[1] = Spectrum::w[1] - v;
    retval.w[2] = Spectrum::w[2] - v;
    retval.w[3] = Spectrum::w[3] - v;
    retval.w[4] = Spectrum::w[4] - v;
    retval.w[5] = Spectrum::w[5] - v;
    retval.w[6] = Spectrum::w[6] - v;
    retval.w[7] = Spectrum::w[7] - v;
    retval.w[8] = Spectrum::w[8] - v;
    retval.w[9] = Spectrum::w[9] - v;
    retval.w[10] = Spectrum::w[10] - v;
    retval.w[11] = Spectrum::w[11] - v;
    retval.w[12] = Spectrum::w[12] - v;
    retval.w[13] = Spectrum::w[13] - v;
    retval.w[14] = Spectrum::w[14] - v;
    retval.w[15] = Spectrum::w[15] - v;
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] - v;
    retval.w[17] = Spectrum::w[17] - v;
    retval.w[18] = Spectrum::w[18] - v;
    retval.w[19] = Spectrum::w[19] - v;
    retval.w[20] = Spectrum::w[20] - v;
    retval.w[21] = Spectrum::w[21] - v;
    retval.w[22] = Spectrum::w[22] - v;
    retval.w[23] = Spectrum::w[23] - v;
    retval.w[24] = Spectrum::w[24] - v;
    retval.w[25] = Spectrum::w[25] - v;
    retval.w[26] = Spectrum::w[26] - v;
    retval.w[27] = Spectrum::w[27] - v;
    retval.w[28] = Spectrum::w[28] - v;
    retval.w[29] = Spectrum::w[29] - v;
    retval.w[30] = Spectrum::w[30] - v;
#endif
    return retval;
}

void Spectrum::operator-=(float v)
{
    Spectrum::w[0] -= v;
    Spectrum::w[1] -= v;
    Spectrum::w[2] -= v;
    Spectrum::w[3] -= v;
    Spectrum::w[4] -= v;
    Spectrum::w[5] -= v;
    Spectrum::w[6] -= v;
    Spectrum::w[7] -= v;
    Spectrum::w[8] -= v;
    Spectrum::w[9] -= v;
    Spectrum::w[10] -= v;
    Spectrum::w[11] -= v;
    Spectrum::w[12] -= v;
    Spectrum::w[13] -= v;
    Spectrum::w[14] -= v;
    Spectrum::w[15] -= v;
#if HQ_SPECTRUM!=0
    Spectrum::w[16] -= v;
    Spectrum::w[17] -= v;
    Spectrum::w[18] -= v;
    Spectrum::w[19] -= v;
    Spectrum::w[20] -= v;
    Spectrum::w[21] -= v;
    Spectrum::w[22] -= v;
    Spectrum::w[23] -= v;
    Spectrum::w[24] -= v;
    Spectrum::w[25] -= v;
    Spectrum::w[26] -= v;
    Spectrum::w[27] -= v;
    Spectrum::w[28] -= v;
    Spectrum::w[29] -= v;
    Spectrum::w[30] -= v;
#endif
}

Spectrum Spectrum::operator*(float v)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] * v;
    retval.w[1] = Spectrum::w[1] * v;
    retval.w[2] = Spectrum::w[2] * v;
    retval.w[3] = Spectrum::w[3] * v;
    retval.w[4] = Spectrum::w[4] * v;
    retval.w[5] = Spectrum::w[5] * v;
    retval.w[6] = Spectrum::w[6] * v;
    retval.w[7] = Spectrum::w[7] * v;
    retval.w[8] = Spectrum::w[8] * v;
    retval.w[9] = Spectrum::w[9] * v;
    retval.w[10] = Spectrum::w[10] * v;
    retval.w[11] = Spectrum::w[11] * v;
    retval.w[12] = Spectrum::w[12] * v;
    retval.w[13] = Spectrum::w[13] * v;
    retval.w[14] = Spectrum::w[14] * v;
    retval.w[15] = Spectrum::w[15] * v;
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] * v;
    retval.w[17] = Spectrum::w[17] * v;
    retval.w[18] = Spectrum::w[18] * v;
    retval.w[19] = Spectrum::w[19] * v;
    retval.w[20] = Spectrum::w[20] * v;
    retval.w[21] = Spectrum::w[21] * v;
    retval.w[22] = Spectrum::w[22] * v;
    retval.w[23] = Spectrum::w[23] * v;
    retval.w[24] = Spectrum::w[24] * v;
    retval.w[25] = Spectrum::w[25] * v;
    retval.w[26] = Spectrum::w[26] * v;
    retval.w[27] = Spectrum::w[27] * v;
    retval.w[28] = Spectrum::w[28] * v;
    retval.w[29] = Spectrum::w[29] * v;
    retval.w[30] = Spectrum::w[30] * v;
#endif
    return retval;
}

void Spectrum::operator*=(float v)
{
    Spectrum::w[0] *= v;
    Spectrum::w[1] *= v;
    Spectrum::w[2] *= v;
    Spectrum::w[3] *= v;
    Spectrum::w[4] *= v;
    Spectrum::w[5] *= v;
    Spectrum::w[6] *= v;
    Spectrum::w[7] *= v;
    Spectrum::w[8] *= v;
    Spectrum::w[9] *= v;
    Spectrum::w[10] *= v;
    Spectrum::w[11] *= v;
    Spectrum::w[12] *= v;
    Spectrum::w[13] *= v;
    Spectrum::w[14] *= v;
    Spectrum::w[15] *= v;
#if HQ_SPECTRUM!=0
    Spectrum::w[16] *= v;
    Spectrum::w[17] *= v;
    Spectrum::w[18] *= v;
    Spectrum::w[19] *= v;
    Spectrum::w[20] *= v;
    Spectrum::w[21] *= v;
    Spectrum::w[22] *= v;
    Spectrum::w[23] *= v;
    Spectrum::w[24] *= v;
    Spectrum::w[25] *= v;
    Spectrum::w[26] *= v;
    Spectrum::w[27] *= v;
    Spectrum::w[28] *= v;
    Spectrum::w[29] *= v;
    Spectrum::w[30] *= v;
#endif
}

Spectrum Spectrum::operator/(float v)const
{
    Spectrum retval;
    retval.w[0] = Spectrum::w[0] / v;
    retval.w[1] = Spectrum::w[1] / v;
    retval.w[2] = Spectrum::w[2] / v;
    retval.w[3] = Spectrum::w[3] / v;
    retval.w[4] = Spectrum::w[4] / v;
    retval.w[5] = Spectrum::w[5] / v;
    retval.w[6] = Spectrum::w[6] / v;
    retval.w[7] = Spectrum::w[7] / v;
    retval.w[8] = Spectrum::w[8] / v;
    retval.w[9] = Spectrum::w[9] / v;
    retval.w[10] = Spectrum::w[10] / v;
    retval.w[11] = Spectrum::w[11] / v;
    retval.w[12] = Spectrum::w[12] / v;
    retval.w[13] = Spectrum::w[13] / v;
    retval.w[14] = Spectrum::w[14] / v;
    retval.w[15] = Spectrum::w[15] / v;
#if HQ_SPECTRUM!=0
    retval.w[16] = Spectrum::w[16] / v;
    retval.w[17] = Spectrum::w[17] / v;
    retval.w[18] = Spectrum::w[18] / v;
    retval.w[19] = Spectrum::w[19] / v;
    retval.w[20] = Spectrum::w[20] / v;
    retval.w[21] = Spectrum::w[21] / v;
    retval.w[22] = Spectrum::w[22] / v;
    retval.w[23] = Spectrum::w[23] / v;
    retval.w[24] = Spectrum::w[24] / v;
    retval.w[25] = Spectrum::w[25] / v;
    retval.w[26] = Spectrum::w[26] / v;
    retval.w[27] = Spectrum::w[27] / v;
    retval.w[28] = Spectrum::w[28] / v;
    retval.w[29] = Spectrum::w[29] / v;
    retval.w[30] = Spectrum::w[30] / v;
#endif
    return retval;
}

void Spectrum::operator/=(float v)
{
    Spectrum::w[0] /= v;
    Spectrum::w[1] /= v;
    Spectrum::w[2] /= v;
    Spectrum::w[3] /= v;
    Spectrum::w[4] /= v;
    Spectrum::w[5] /= v;
    Spectrum::w[6] /= v;
    Spectrum::w[7] /= v;
    Spectrum::w[8] /= v;
    Spectrum::w[9] /= v;
    Spectrum::w[10] /= v;
    Spectrum::w[11] /= v;
    Spectrum::w[12] /= v;
    Spectrum::w[13] /= v;
    Spectrum::w[14] /= v;
    Spectrum::w[15] /= v;
#if HQ_SPECTRUM!=0
    Spectrum::w[16] /= v;
    Spectrum::w[17] /= v;
    Spectrum::w[18] /= v;
    Spectrum::w[19] /= v;
    Spectrum::w[20] /= v;
    Spectrum::w[21] /= v;
    Spectrum::w[22] /= v;
    Spectrum::w[23] /= v;
    Spectrum::w[24] /= v;
    Spectrum::w[25] /= v;
    Spectrum::w[26] /= v;
    Spectrum::w[27] /= v;
    Spectrum::w[28] /= v;
    Spectrum::w[29] /= v;
    Spectrum::w[30] /= v;
#endif
}

StratifiedSpectrum::StratifiedSpectrum(float val, unsigned char index)
{
    memset(StratifiedSpectrum::w, 0, sizeof(float)*SPECTRUM_SAMPLES);
    StratifiedSpectrum::chosen = index;
    w[chosen] = val;
}

StratifiedSpectrum StratifiedSpectrum::stratify(float, float*)const
{
    return *this;
}

Spectrum StratifiedSpectrum::weight()const
{
    Spectrum retval;
    memset(retval.w, 0, sizeof(float)*SPECTRUM_SAMPLES);
    retval.w[StratifiedSpectrum::chosen] = 1.f;
    return retval;
}

unsigned char StratifiedSpectrum::getComponent()const
{
    return StratifiedSpectrum::chosen;
}
