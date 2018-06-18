#include "ebu/list/serialization/compliance.h"

using namespace ebu_list;
using namespace ebu_list::st2110;

//------------------------------------------------------------------------------

d21::compliance_analyzer d21::build_compliance_analyzer(const d20::video_description& video, d21::vrx_settings settings)
{
    return d21::compliance_analyzer(video.packets_per_frame,
        video.rate,
        d20::get_info(video),
        settings,
        video.scan_type,
        video.dimensions);
}

d21::video_analysis_info d21::get_video_analysis_info(const st2110::d21::compliance_analyzer& ca)
{
    d21::video_analysis_info va;

    const auto compliance = ca.get_compliance();

    va.compliance = compliance.global;

    va.cinst = d21::cinst_analysis{
        compliance.cinst,
        transform_histogram_samples_to_percentages(ca.get_cinst_histogram())
    };

    va.vrx = d21::vrx_analysis{
        compliance.vrx,
        transform_histogram_samples_to_percentages(ca.get_vrx_histogram())
    };

    return va;
}

void d21::to_json(nlohmann::json& j, const video_analysis_info& v)
{
    j["compliance"] = v.compliance;
    j["cinst"] = v.cinst;
    j["vrx"] = v.vrx;
}

void d21::from_json(const nlohmann::json& j, video_analysis_info& v)
{
    v.compliance = j.at("compliance");
    v.cinst = j.at("cinst");
    v.vrx = j.at("vrx");
}

void d21::to_json(nlohmann::json& j, const cinst_analysis& v)
{
    j["compliance"] = v.compliance;
    j["histogram"] = v.histogram;
}

void d21::from_json(const nlohmann::json& j, cinst_analysis& v)
{
    v.compliance = j.at("compliance");
    //v.histogram = j.at("histogram").get<std::map<int,int>>(); // todo: uncomment and make it compile
}

void d21::to_json(nlohmann::json& j, const vrx_analysis& v)
{
    j["compliance"] = v.compliance;
    j["histogram"] = v.histogram;
}

void d21::from_json(const nlohmann::json& j, vrx_analysis& v)
{
    v.compliance = j.at("compliance");
    //v.histogram = j.at("histogram").get<std::map<int,int>>(); // todo: uncomment and make it compile
}

void st2110::d21::to_json(nlohmann::json& j, const compliance_profile& v)
{
    switch (v)
    {
    case compliance_profile::narrow: j = "narrow"; break;
    case compliance_profile::narrow_linear: j = "narrow_linear"; break;
    case compliance_profile::wide: j = "wide"; break;
    default:
        LIST_ASSERT(v == compliance_profile::not_compliant);
        j = "not_compliant";
        break;
    }
}

void st2110::d21::from_json(const nlohmann::json& j, compliance_profile& v)
{
    const auto s = j.get<std::string>();

    if (s == "narrow") v = compliance_profile::narrow;
    else if (s == "narrow_linear") v = compliance_profile::narrow_linear;
    else if (s == "wide") v = compliance_profile::wide;
    else
    {
        LIST_ENFORCE(s == "not_compliant", std::runtime_error, "invalid JSON value for compliance_profile");
        v = compliance_profile::narrow;
    }
}
