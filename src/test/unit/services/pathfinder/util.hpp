#ifndef TEST_UNIT_SERVICES_PATHFINDER_UTIL_HPP
#define TEST_UNIT_SERVICES_PATHFINDER_UTIL_HPP

#include <stan/callbacks/interrupt.hpp>
#include <stan/callbacks/stream_writer.hpp>
#include <iostream>
#include <memory>

namespace stan {
namespace test {

struct mock_callback : public stan::callbacks::interrupt {
  int n;
  mock_callback() : n(0) {}

  void operator()() { n++; }
};

class test_logger : public stan::callbacks::logger {
  std::unique_ptr<std::ostream> log_;

 public:
  test_logger(std::unique_ptr<std::ostream>&& x) : log_(std::move(x)) {}
  test_logger() : log_(std::unique_ptr<std::ostream>(nullptr)) {}
  /**
   * Logs a message with debug log level
   *
   * @param[in] message message
   */
  virtual void debug(const std::string& message) { 
    if (log_ != nullptr) {
      *log_ << message << "\n"; 
    }
  }

  /**
   * Logs a message with debug log level.
   *
   * @param[in] message message
   */
  virtual void debug(const std::stringstream& message) {
    if (log_ != nullptr) {
      *log_ << message.str() << "\n";
    }
  }

  /**
   * Logs a message with info log level.
   *
   * @param[in] message message
   */
  virtual void info(const std::string& message) { 
    if (log_ != nullptr) {
      *log_ << message << "\n"; 
    }
  }

  /**
   * Logs a message with info log level.
   *
   * @param[in] message message
   */
  virtual void info(const std::stringstream& message) {
    if (log_ != nullptr) {
      *log_ << message.str() << "\n"; 
    }
  }

  /**
   * Logs a message with warn log level.
   *
   * @param[in] message message
   */
  virtual void warn(const std::string& message) { 
    if (log_ != nullptr) {
      *log_ << message << "\n"; 
    }
  }

  /**
   * Logs a message with warn log level.
   *
   * @param[in] message message
   */
  virtual void warn(const std::stringstream& message) {
    if (log_ != nullptr) {
      *log_ << message.str() << "\n"; 
    }
  }

  /**
   * Logs an error with error log level.
   *
   * @param[in] message message
   */
  virtual void error(const std::string& message) { 
    *log_ << message << "\n"; 
  }

  /**
   * Logs an error with error log level.
   *
   * @param[in] message message
   */
  virtual void error(const std::stringstream& message) {
    if (log_ != nullptr) {
      *log_ << message.str() << "\n";
    }
  }

  /**
   * Logs an error with fatal log level.
   *
   * @param[in] message message
   */
  virtual void fatal(const std::string& message) { 
    if (log_ != nullptr) {
      *log_ << message << "\n"; 
    }
  }

  /**
   * Logs an error with fatal log level.
   *
   * @param[in] message message
   */
  virtual void fatal(const std::stringstream& message) {
    if (log_ != nullptr) {
      *log_ << message.str() << "\n";
    }
  }
};

/**
 * Writer that stores results in memory.
 */
class in_memory_writer : public stan::callbacks::stream_writer {
 public:
  std::vector<std::string> names_;
  std::vector<std::string> times_;
  std::vector<std::vector<double>> states_;
  std::vector<Eigen::VectorXd> eigen_states_;
  std::vector<std::tuple<Eigen::VectorXd, Eigen::VectorXd>> optim_path_;
  Eigen::MatrixXd values_;
  in_memory_writer(std::ostream& stream)
      : stan::callbacks::stream_writer(stream) {}

  /**
   * Writes a set of names.
   *
   * @param[in] names Names in a std::vector
   */
  void operator()(const std::vector<std::string>& names) { names_ = names; }

  void operator()(const std::string& times) { times_.push_back(times); }
  void operator()() { times_.push_back("\n"); }
  /**
   * Writes a set of values.
   *
   * @param[in] state Values in a std::vector
   */
  void operator()(const std::vector<double>& state) {
    states_.push_back(state);
  }
  void operator()(
      const std::vector<std::tuple<Eigen::VectorXd, Eigen::VectorXd>>& xx) {
    optim_path_ = xx;
  }
  void operator()(const std::tuple<Eigen::VectorXd, Eigen::VectorXd>& xx) {
    optim_path_.push_back(xx);
  }
  template <typename EigVec, stan::require_eigen_vector_t<EigVec>* = nullptr>
  void operator()(const EigVec& vals) {
    eigen_states_.push_back(vals);
  }
  template <typename EigMat,
            stan::require_eigen_matrix_dynamic_t<EigMat>* = nullptr>
  void operator()(const EigMat& vals) {
    values_ = vals;
  }
};

Eigen::Matrix<double, 10, 100> eight_schools_r_answer() {
  Eigen::Matrix<double, 10, 100> r_answer;
  r_answer << -0.480685045806342, 0.264657333844428, -0.87050253936714,
      1.42596346969073, 0.443619146321916, -0.974978025140829,
      0.361502911648785, 0.681564799788363, 1.87316921975915,
      0.0890479151877764, 1.66784640030806, 0.479372088959062,
      -0.657194387122538, -2.34599552631952, 1.23282178602114,
      -0.222741207557055, -0.141225704483779, 0.894571347685905,
      2.41598320562301, 1.86329289834444, 1.07685860726025, 0.871072276564516,
      0.0374543053586734, -2.12395432344545, 0.731787243096492,
      -0.234156038348887, -0.278711974515752, -1.51234913916009,
      1.10705411729555, 1.67181882197232, 1.50569365545024, -0.0175631902092582,
      0.350686963104951, -0.206162574563027, -1.25104267612603,
      -0.579905953804618, -0.507034066363174, -0.109600907710341,
      2.53090680056458, 1.57420806791459, -0.0192143245189976,
      -0.17114476510022, 0.65978017649587, 0.400594068097216,
      -0.567664564239315, -0.863740946634223, 0.240970421840771,
      0.712531598891873, 1.20336280090677, 1.39094909268465, 0.548625793155492,
      -0.525507386241689, 0.304039619415891, -1.26882199271062,
      1.53904319775136, 1.78766753955081, -0.481526904275361, -1.09137990053433,
      2.00306124499911, 0.650292039081524, 2.54992482733305, 0.0481776293005072,
      0.65218243400968, -0.128033276861408, -0.622437267124869,
      0.0136000245073658, -1.8934623677001, 1.41277290580056, 1.66205557773828,
      3.28092037954096, 0.622871206075383, -0.624852936702337,
      0.573320853499647, -1.074209571022, -1.12762223557757, 0.120066163015778,
      -0.554909726708834, -0.0512388994567235, 1.44891025153395,
      0.0170786611526343, -0.418276394314608, -0.0510269787171272,
      1.13990301280083, -1.65569745203382, 0.705909094399899, 0.16134042505005,
      0.924221716314448, -0.355036569228519, 1.75431652089165,
      0.935348861365577, -0.388859621777651, 1.29154710207532, 1.12192489726007,
      0.536277257325521, 1.69224886427522, 0.380873425107417, -1.37495454785214,
      -0.622314432729262, 0.200342089430146, 0.299783529582894,
      -0.480126349556509, 0.12037891904654, -0.945182548794795,
      0.0121198923491542, -0.534310007014266, 1.58955016884842,
      0.590614145740349, 0.851187988398187, 1.59738038984837, 1.9318524155075,
      -0.483698785781882, -0.376147306437035, 1.39358646902146,
      -0.795359593024569, -0.0893180317193737, -0.557775450567792,
      -0.434418022254071, -0.329555917463719, 1.90192406372771,
      0.535074639123403, -0.357279734867408, 1.42384177634192,
      -0.25063609621847, -0.328406761719355, 0.017358976631599,
      0.537917331619613, -0.191203597317852, -0.0905523715414699,
      0.662848445702064, 0.220843732152141, 0.218784730740073,
      -0.498539315092672, 0.493710612664504, -1.65810485015338,
      0.421083012724649, -1.69657940629592, -0.420587235864792,
      -0.574027482711467, 0.898431234287898, 1.08042524730608,
      -1.76838019013617, 1.25404572371693, -1.69860354829011,
      -0.604505710724831, -0.990173020053883, -0.903602118641434,
      1.93256666002767, -0.0395863701467866, -0.0743062226776963,
      -1.51930565483581, 0.599037370283978, 0.0661205733301442,
      -0.353904550531436, -1.06975839970584, -1.36073498510499,
      -1.2320027438659, 0.862116776057241, -0.669966024898478,
      0.0216008677551769, 2.69719184978421, 0.564557525051461,
      -0.158720799475554, 1.02088168640598, 0.728791934563778,
      -0.498315491100515, 2.02093350924027, -0.365346178135276,
      -1.47560275865866, 1.09198291094004, 0.463917824171489, 2.44921625809493,
      0.188997517182353, 0.420118438713606, -0.226791551689285,
      -0.215904363885426, -0.20091239252185, 0.655070141755858,
      2.01671088926715, 2.40678152929406, 1.89497200536938, -1.07902656172774,
      1.06615893451854, 0.183057934089501, -1.60074851666053, 0.634305198978641,
      -0.325376305413676, 1.31749023675859, -0.816125234275669,
      0.939357455709719, -0.341266340874469, -0.0331894801235708,
      0.48233922987997, -0.766766634986172, 0.671338163610171,
      -1.08262501966774, -1.19946620391565, 1.29791737677625, -1.06717775694571,
      1.69725688052396, -0.0217288575520471, 0.544449801254452,
      1.76310152630717, 1.54845000689837, -0.469366861511199, -2.15234344821202,
      2.31344595205094, 0.542429882965011, 0.481865365885787, 1.14813374858918,
      0.567250411318214, -0.0106704697058941, 0.506317611638856,
      -0.436252099225911, -1.50764228064347, 1.09707511873831, 1.33073580538245,
      -0.424926250315224, -1.29904179146989, 2.10493569167943,
      0.826353031169728, -1.58602720685632, 0.0825249212470953,
      -0.665360662032109, -0.483784815242552, -1.03177153694833,
      1.62104052030781, -0.440846930103781, -1.65518971883847, 1.46510092319151,
      0.60581646455029, -0.840256991362053, -2.80333520505605,
      -0.675545782895745, 0.415538850654104, 0.0570027462435979,
      -0.259496016401498, 0.434446844805491, -1.23725018751362,
      2.37865400913385, 0.351149306835651, 0.869280868985328, 1.12450491114209,
      0.186036521910883, -1.0299244113961, 1.27058467821062, -2.15945401171363,
      -0.662049198443, -0.29926687826387, 1.48773127699589, 2.49033239728325,
      0.290873030432002, 0.493789170079176, -0.106313949112642,
      -0.401139007383008, 0.807185033680802, 0.95959956332433,
      -2.48195176574024, 0.521610204164376, 1.86441693705977, 0.50870058285062,
      1.10090333577335, -0.302587020311158, -0.320615806537165,
      0.691044274736706, 1.82414476809577, 0.0951939293189129,
      -0.536603498704663, -1.23598670336725, 1.10580533781152,
      -0.16750855461308, -0.113473590457408, 0.475430006291848,
      -0.886688768530324, 2.46586809754062, 0.271132774825349,
      0.953421594456443, -2.36405353576345, 0.68482435318347,
      -0.0121875880493223, 1.41649099708462, 0.550434900911507,
      -0.320392396559432, 1.28569174236642, -0.847049986775028,
      -0.460414363418081, -1.1617949923671, -0.777286593778772,
      0.892798086396931, 1.90633991016588, 1.95471035558827, -0.236977084716238,
      0.46141718065705, 0.207175246506649, -1.5622955753859, 1.88210747533448,
      -0.0381531827847733, 0.630030083315715, 0.901326068101437,
      1.38369214717797, 0.483654445460783, 1.03849340338656, -0.962366874243989,
      1.93195394190204, -0.529701537165328, 1.75855506582096, 1.32860485788533,
      -0.0373347546711421, 0.512613130642201, 0.323827211969626,
      0.913323127877251, 1.19168528527445, 0.184051560057684,
      -0.489957402250425, -0.800940952825897, 0.0808751633235408,
      0.88716015329148, -0.595800029223496, -0.171586398336362,
      0.118345335879769, 1.2770449943391, 1.46223081563771, 1.58343096957188,
      0.776593897207152, -2.00411046374169, 0.594899362517853,
      0.274681577255158, -0.471257434077962, 0.122420234474059,
      0.677019368858838, 1.82622686404355, -0.189747697675016,
      -1.48918502374322, -0.402714155425066, 1.20332929288671,
      -0.215060497807401, 0.567643599999574, 0.816020194601884,
      -0.0537849525106505, 0.802313613279164, -0.533846908738354,
      0.884513562069069, -0.978683228348959, 0.209756633418067,
      -0.435604831120983, -2.13292809868895, -1.55882915877373,
      0.781806120484083, -0.242445725455071, 2.1722307070144, 2.57910294380311,
      1.62686000999366, 0.764579895494398, 0.342431985424922,
      -0.348765298940495, 1.68339121643307, 0.413611538359572,
      -1.27655632400488, -0.203676413406571, -0.408441792245902,
      0.841807399831808, -2.44381556681993, 1.39156112658709,
      -0.670740093808337, -0.571615143449588, -0.0507840367040222,
      0.443829185772766, 0.549626143999177, 0.509698560959641,
      0.657843026606138, -1.18828839689536, -0.24084255770389,
      0.360273781831064, -0.857925937326108, -0.217843783634848,
      -1.04348607203289, -0.173858338519966, 0.00898907161018794,
      -0.197551379433561, 1.19190796441216, 2.42700169443012, 0.919574098123312,
      1.19908496821371, -0.958894918550507, 0.00205408002300547,
      1.26376547496767, -0.23387582362662, -2.21616307280506, 0.29580883157942,
      -0.363699927879275, 0.284670127826115, 1.46762664473163,
      0.697746800781487, 1.05371902664561, 0.151859104891517,
      0.00682749359623329, -1.08052594555925, 1.44518444565323,
      -0.00753115706460054, 0.640420204122651, 1.49560392273226,
      1.22673337441755, 1.97990388008222, -0.638831343035132,
      -0.542637087742618, -0.296510634113825, -0.545526350625818,
      -0.483056441754797, -0.343734871060401, 2.94360767988161,
      0.242335928798137, -0.242054588662589, 0.733302113127251,
      1.08695475671399, -0.910936892439307, -0.388490288620053,
      0.353583565322699, 0.882202420868273, -0.30357599705529,
      -0.115867713020107, 2.27148148256022, 1.58883441123674,
      -0.620330059319558, -0.101949619030153, -1.8956206077098,
      0.682641320976544, 1.41775594826672, -1.73210909949777,
      -0.825248371773916, 0.887759017997818, 0.367025618720097,
      -1.8819598677465, 0.582287607005448, -1.565628275146, -0.174243012529707,
      0.70493058581987, -0.360937130870223, 0.758031966512155,
      -0.0789986061244641, 0.674409036231428, 1.17151040359521,
      -0.27268395789196, 1.85955847932699, -0.0554656332582127,
      0.680738728858028, 0.321464042291368, -3.15340261074618,
      -1.46778730844021, -0.470604679227321, 1.79759508949568,
      -1.29282274852158, 1.11108670271281, -0.51766656321784,
      -0.788278510743547, -1.68982897329352, -1.32675451615694,
      -0.115315647135397, 0.378738279666496, -2.14138422266348,
      0.429806060752618, 1.37716110303777, -0.302819810452274, 2.24568263475833,
      1.20758601106554, 0.436298448314298, 0.120511198821218, 0.108608497476698,
      -0.811616575045726, 0.574778051431439, 2.88229833888187, 1.82208817146235,
      -0.661002894361026, -1.53157180001537, -0.145911319604916,
      0.282450077665773, 1.45741389271268, -1.4728459169234, 0.237737650106095,
      0.181421406222975, 2.6030006870674, 0.684499232141329, -0.208549628977592,
      -1.06150449863511, -0.553003108322036, -0.508839303061984,
      2.44997197655416, 2.25207568269402, -0.282735312613392, -1.0929031015824,
      -0.624385490468558, 1.12825118886407, -0.927083599418688, 2.3709920098329,
      -0.921351549837319, -0.0510963555044156, 3.89936861493994,
      -1.2754356938545, 0.175607961523926, -1.15022386174237, 1.92817772983995,
      0.296874737051136, 0.228055155194354, -0.211251876552043,
      -1.21801347800031, -0.142780473563928, 1.10124822028301, 1.40592789977844,
      -1.46854544588983, -0.300150046342377, 2.57841657409131,
      -0.422268571217983, -2.37781621630771, -0.853241457784229,
      -1.00176498355218, -0.100955690608104, -0.284833458969901,
      0.0660336880984902, -0.530685176016623, 0.318870068913376,
      0.926181298737209, 1.651369134923, -0.58377891974788, 0.375545201254722,
      -0.91998492721862, 0.0514987419432182, 0.0689525596761908,
      -1.83605845763816, -0.26232781217582, 1.12574956566244, 2.11478088105998,
      0.939597381648194, -1.03675554592877, 1.25300402110087,
      0.0424854960754904, -0.600453157617244, 1.74658078770239,
      -0.932160218149132, 0.176354485129137, 1.22703907485963, 2.04429743897819,
      0.494696067807857, -0.265093253174497, 0.440274674349925,
      0.476058857130949, -0.136283931933683, 1.42555699406597,
      -0.236324767162488, -0.808990145452859, 0.484887812296702,
      -0.777848370539061, 1.2158763423729, 0.658172556661827,
      -0.133064694202513, -1.1526258506103, -0.55219803802225, 1.65547735355631,
      -0.916119772932417, -2.41624292569042, 0.763694456439439,
      1.11459490509839, 0.787960308781442, -1.06737771568024,
      0.0591025344399154, 0.664042753119438, -0.728583746590817,
      -0.486131980713224, 0.920469021062525, -0.359801616724539,
      -0.21309243858636, 0.680375027470079, 1.39186635169015,
      -0.849047988644344, 0.928231609720267, 0.768187056204372,
      -0.609706112060453, 0.959647686363666, 0.813046720813862,
      0.448007498293625, 1.9622068927077, -0.720901131626094,
      -0.929074397110259, -1.23393835167381, 0.245841322159118,
      1.54623545964145, 1.51110562147414, 0.601485912960762, 0.709886494672577,
      -0.256559755114272, 0.410334565068892, 1.57473458604648,
      -0.662869109087838, -0.146068002298644, -0.0519817160217654,
      1.35536562944651, -1.17404485485496, -1.98209611751246, 0.593733314858779,
      -0.919582000668085, -0.488039315071744, 2.24376721989811,
      -0.276751532349524, -0.18400835004327, 1.58775036818712, 0.49080596565308,
      0.378235175739529, -0.01958599245279, -1.29844864606835,
      -1.59206054461157, -0.270792617814048, 3.52789741767833, 1.263436844276,
      0.327284345625146, 0.423894225842222, -0.421827236056127,
      -1.54335843872965, 2.77296984774811, -0.600454226811531,
      -0.420805435548274, -1.83182250121119, 1.38489833433015, 1.10756781851451,
      -0.111315092915812, -1.70285410732519, 1.74591114177835, 1.59300863997365,
      0.800373423598484, -1.25122581278837, 0.585404725700977,
      -0.299601258426976, 0.973311534517382, 1.83357332256063,
      -1.08233508502495, -0.430066556261621, -0.766096900324613,
      -0.127621735459342, -1.44496932255577, -0.857293389161872,
      0.58701331226678, 0.408592877482515, 0.283492379289786, 0.903036852610279,
      -2.27636634103076, -0.255657325236275, 0.675448176074138,
      -0.803147544208506, 0.0806174930289512, -1.74420563922774,
      0.71210874069581, -1.89626961158259, 1.06157399594953, 0.402386155604078,
      0.220378740571303, -0.732285277707248, 1.7899636467771, -1.57111695271474,
      0.36872438461105, -3.08480644775821, -0.122894883924879,
      0.461201443607518, 0.48287193343777, 1.93105613440314, 0.484970563808973,
      1.06191771694946, -0.878522462572055, -1.11354869912451,
      -1.64313677312062, -0.491801257134673, -1.43580451620764,
      0.637830304925711, 1.57623418792463, 3.28920853347629, -0.992971159424353,
      1.27047202048005, -0.563727019877892, -1.59932433445303,
      0.685897275032786, -1.59729527421228, -1.16271192512023,
      -0.777245006393334, 1.80827095529974, 0.215045968145435, 1.2232954074641,
      -1.11316199802544, 0.179863591985604, -0.010794784519502,
      -0.941178951633815, -0.591739423710262, -0.763701696499033,
      0.903855841943717, 2.93559853684942, -0.462602658563341, 1.08902059474117,
      0.0152762655069578, -2.0012150572451, -0.907054242609159,
      0.574433197945852, -0.0358079049817879, -2.52361898363155,
      0.532119279794941, 2.254701010096, 0.94060488764036, -0.548352012954269,
      2.05670299552344, -0.12646304819549, -0.171028892998702, -0.9998039671806,
      -1.50413982670526, -1.61800253566305, -0.467567027684119,
      2.91302394734243, 2.94723453875257, 1.72595885219719, 1.38565529404297,
      -0.274199702569317, -1.369388557667, -0.209401616468346,
      -2.56528861298251, -0.434112841471977, 1.60919419626146, 1.75401800788685,
      2.47797224205225, 0.637383845531567, -0.69330956530309, 1.70440947186789,
      -0.229949535824643, -0.851786715211304, -0.0987880056788862,
      -1.61059626114178, 0.809130370739691, 1.89405255887058, 0.306529634393207,
      -0.335008481321958, 1.02038490123121, -1.09724333051014,
      -1.12507436510987, 0.537199144711449, -0.616976341686777,
      0.787266116794213, -0.248028476386195, 2.64496285904445, 1.34728480459258,
      -2.09742403951238, -1.25320009571081, 1.24503697975747, 0.536137189192038,
      -0.844773644758746, -1.49124266840415, 0.900477208990653,
      -0.863466972080605, 3.07756754792064, 2.18067632939517, -1.31134737453519,
      -0.765474211359249, -1.28448981008064, 0.51334277009697,
      -1.16468031962072, -2.17167342697816, 1.86964914942172, 0.945969906056422,
      2.0297142272377, -0.528294433256265, 0.13031837197617, 0.698635366567805,
      -1.3154008029194, -0.269927315443494, 0.291463055868969, 1.51213050113387,
      0.514161145572669, 1.22208203421386, 1.38577949388567, -0.894969907849343,
      -0.19754773138353, -1.58213771267596, 0.891515929907153, 1.25230507545073,
      0.0536827708157103, -1.13069595094793, 0.952307569894461,
      0.0837243707266382, 0.863966329022897, 1.46393825572648,
      -0.605666354220166, 1.23268531447786, -0.877810541529725,
      0.23172132756424, 1.00167846447823, -1.49151783972955, 0.269206080369976,
      -0.162000746683493, 2.11206161675689, -0.0259219533641473,
      0.0642920313975898, 1.46507233373345, 0.131728006552689, 0.65892968138703,
      -0.102837978053917, -1.18791482429624, -0.131358865352399,
      -1.27184672448733, -1.18911795648054, 1.98621491651773,
      -0.940592372723748, -1.74205430134783, 0.957470605669522,
      -0.160361567056103, -0.477984945739609, -0.339372644898647,
      -0.535426433241227, 0.938029479070316, 2.00239737582892,
      -1.46240075750977, 0.502060645862483, 0.809977085828968,
      0.630760732742147, -2.54562461738252, -0.117023169973342,
      1.78866947383433, 0.662047828140661, -1.75535125444071,
      -0.278455672243674, 1.20234432696698, -0.0239367631181966,
      1.04403472659246, 0.256867553607498, -0.0729179517683806,
      0.299112075555256, -0.00455168679705592, -1.36648002914011,
      2.28988040335999, -0.0287976605496425, 0.661957062814105,
      -0.403538818308465, 1.87579773977282, 0.973825398206342,
      -0.702130682943231, 0.321065613825578, 0.992736868593171,
      2.08010628735946, 0.244092772330871, 0.15308575163891, -0.882459370632329,
      2.14410639300836, -1.98567574370401, 3.0148260942962, -0.406752655982078,
      -0.334398118049327, -0.00801537531804938, 0.799073168526317,
      0.247415701108991, -0.672625252752768, 0.776353130712932,
      0.611985140550054, 0.749971998353841, -1.06702076586645,
      -2.50265794144684, -0.20530362335999, -1.11478417498548,
      -0.879233176345936, -0.996303698101789, 1.2240752569151, 1.01249718014898,
      0.944888693901318, 1.06990223501136, -0.829835898769348,
      -0.458524164678054, 0.475368012325437, 1.2130664537499,
      -0.175827500384662, -1.74428944123854, 1.65222697149098,
      0.635467409690308, 1.91761119726471, 0.427497606558801,
      -0.658587289330163, -0.586938290539023, -0.384944163792527,
      0.0146307676289951, 0.0527965914270619, 0.861335603834235,
      1.72385064397625, 0.328324753588675, -0.79256733727735, 0.715699643084081,
      -0.0986263040522766, 0.0308485024743869, 1.21207029844484,
      1.56755827044655, -1.05395004827659, 1.58500669799276, 0.447667793469686,
      0.756016953805816, 0.373891822926807, -0.190684597524466,
      1.35537710202994, -0.805380522281356, 0.772915299800455,
      -0.184618332919013, -1.03639029944381, 1.15961649297601,
      -0.669862462312345, 0.239270643318379, -1.38606577042523,
      0.279100003421741, 0.227418582526803, -1.25814017960437,
      0.763082927671313, -1.19161134449968, 0.52597801642388, 0.18570168924734,
      2.13210502611633, -0.238079234985063, 0.250132911333618,
      -0.374714340489767, -1.50313756862187, -0.00505868740816873,
      1.8763072215619, -0.813972047434684, -0.318019850784429,
      0.638822038902558, 1.54073940914917, 3.03139421358819, 0.897677020495192,
      2.40876845458324, 0.31131813780345, -1.28090567049677, 0.534520609752711,
      -1.09463092880191, -1.1151755293416, -0.234081967465431, 2.56658088069084,
      1.70715996178485, 0.966018589314684, 1.00070186127327, -0.208401132084604,
      -2.52756816407708, 0.907216331931165, 1.97148809547862,
      -0.0658217273848589, -1.40043459569768, 1.09944897988561,
      -0.0764130898652789, 0.0951576723636569, 0.752320578214279,
      0.829363492548964, 0.0889516259083183, -0.810881172285029,
      0.647339567945083, 1.2107727738968, 2.18691977183703, 0.672156315623124,
      0.548814951531403, 1.33434487034617, -0.03484287520089, 0.487632075572401,
      0.0620277441926289, -0.0157264751292767, 0.00219244683097661,
      0.82874474219177, 0.352011351509342, 0.80930486651947, -1.97807106749011,
      -0.652856897560237, 0.0592150117754701, -2.54649445849421,
      2.03656397564565, -1.36134350462674, -1.30776929247158, 1.31615154363594,
      -2.2389090459583, 1.9659658474394, -0.152385472702995, 0.602274275509886,
      0.959943599797605, -0.99323844537079, -0.847593351812441,
      -2.85890197675248, -1.11707014879034, 0.254739263944208,
      0.451273091807035, 3.40437860863757, 0.679401443062712, 0.613569550800485,
      0.159896399846877, -0.522514889048341, 0.585784730008299,
      0.576871525604595, -0.0419728014492359, -0.920778465376809,
      -0.0904476642241298, 2.21521139466107, 1.49652792835141,
      -0.333783754073736, -0.58474313916949, 0.475533076117177,
      0.883659297466583, 0.237348359431015, -0.477046037210403,
      -0.983966078658822, -0.471162152607535, -0.142998279988775,
      -0.170498289396448;
  return r_answer;
}

std::pair<Eigen::Matrix<double, 10, 1>, Eigen::Matrix<double, 10, 1>>
normal_glm_param_summary() {
  Eigen::Matrix<double, 10, 1> mean_param_vals;
  mean_param_vals << 20.1766, -7191.17, -3.99891, -2.00917, 0.00428024,
      0.990037, 2.98999, -1.02273, 1.01613, -0.994856;
  Eigen::Matrix<double, 10, 1> sd_param_vals;
  sd_param_vals << 1.83697, 1.85228, 0.0145116, 0.0145324, 0.0136457, 0.0137487,
      0.0144594, 0.0145803, 0.0102204, 0.0146323;
  return {mean_param_vals, sd_param_vals};
}
}  // namespace test
}  // namespace stan

#endif
