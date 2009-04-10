/***********************************************************************
Moses - factored phrase-based language decoder
Copyright (C) 2009 University of Edinburgh

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
***********************************************************************/

#include <fstream>
#include "FeatureFunction.h"
#include "Model1.h"

namespace Josiah {

FeatureFunction::~FeatureFunction(){} // n.b. is pure virtual, must be empty



void configure_features_from_file(const std::string& filename, feature_vector& fv){
  std::cerr << "Reading extra features from " << filename << std::endl;
  std::ifstream in(filename.c_str());
  // todo: instead of having this function know about all required options of
  // each feature, have features populate options / read variable maps /
  // populate feature_vector using static functions.
  po::options_description desc;
  bool useApproxPef = false;
  bool useApproxPfe = false;
  desc.add_options()
    ("model1.table", "Model 1 table")
    ("model1.pef_column", "Column containing p(e|f) score")
    ("model1.pfe_column", "Column containing p(f|e) score")
    ("model1.approx_pef",po::value<bool>(&useApproxPef)->default_value(false), "Approximate the p(e|f), and use importance sampling")
    ("model1.approx_pfe",po::value<bool>(&useApproxPfe)->default_value(false), "Approximate the p(f|e), and use importance sampling");
  po::variables_map vm;
  po::store(po::parse_config_file(in,desc,true), vm);
  notify(vm);
  if (!vm["model1.pef_column"].empty() || !vm["model1.pfe_column"].empty()){
    boost::shared_ptr<external_model1_table> ptable;
    boost::shared_ptr<moses_factor_to_vocab_id> p_evocab_mapper;
    boost::shared_ptr<moses_factor_to_vocab_id> p_fvocab_mapper;
    if (vm["model1.table"].empty())
      throw std::runtime_error("Requesting Model 1 features, but no Model 1 table given");
    else {
      ptable.reset(new external_model1_table(vm["model1.table"].as<std::string>()));
      p_fvocab_mapper.reset(new moses_factor_to_vocab_id(ptable->f_vocab(), Moses::Input, 0, Moses::FactorCollection::Instance())); 
      p_evocab_mapper.reset(new moses_factor_to_vocab_id(ptable->e_vocab(), Moses::Output, 0, Moses::FactorCollection::Instance())); 
    }
    if (!vm["model1.pef_column"].empty()) {
      if (useApproxPef) {
        cerr << "Using approximation for model1" << endl;
        fv.push_back(feature_handle(new ApproximateModel1(ptable, p_fvocab_mapper, p_evocab_mapper)));
      } else {
        fv.push_back(feature_handle(new model1(ptable, p_fvocab_mapper, p_evocab_mapper)));
      }
    }
    if (!vm["model1.pfe_column"].empty()) {
      if (useApproxPfe) {
        cerr << "Using approximation for model1 inverse" << endl;
        fv.push_back(feature_handle(new ApproximateModel1Inverse(ptable, p_fvocab_mapper, p_evocab_mapper)));
      } else {
        fv.push_back(feature_handle(new model1_inverse(ptable, p_fvocab_mapper, p_evocab_mapper)));
      }
    }
  }
  in.close();
}



FeatureFunctionScoreProducer::FeatureFunctionScoreProducer(const std::string & name ) :m_name(name){
  StaticData& staticData = const_cast<StaticData&>(StaticData::Instance());
  const_cast<ScoreIndexManager&>(staticData.GetScoreIndexManager()).AddScoreProducer(this);
  vector<float> w(1); //default weight of 0
  staticData.SetWeightsForScoreProducer(this,w);
}



size_t FeatureFunctionScoreProducer::GetNumScoreComponents() const {
  return 1;
}

string FeatureFunctionScoreProducer::GetScoreProducerDescription() const {
  return m_name;
} 
 
}//namespace
