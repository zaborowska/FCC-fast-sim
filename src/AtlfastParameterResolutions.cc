#include "AtlfastParameterResolutions.hh"
#include "FCCPrimaryParticleInformation.hh"
#include "G4PrimaryParticle.hh"
#include <cmath>

namespace Atlfast
{

   double ParameterResolutions::resolution( const G4Track& track ) const
   {
      FCCPrimaryParticleInformation* info = (FCCPrimaryParticleInformation*) track.GetDynamicParticle()->GetPrimaryParticle()->GetUserInformation();

      // resolutions are given by c0(eta) + c1(eta)/sqrt(pT) + c2(eta)/pT + c3(eta)/pT/sqrt(pT) + c4(eta)/pT^2
      // have to interpolate eta between bin edges for coefficients
      double eta = std::abs( info->GetMomentumProduction()->pseudoRapidity() );
      double pT  = info->GetMomentumProduction()->perp();

      std::vector<BinID>::const_iterator iter = m_coefficientBins.begin();
      std::vector<BinID>::const_iterator end  = m_coefficientBins.end();
      std::vector<double> coefficients;
      coefficients.reserve( m_coefficientBins.size() );
      for ( ; iter != end; ++iter )
      {
         double value = this->interpolate( eta, iter->low(), iter->high() );
         coefficients.push_back(value);
      }
      return calculateResolution( pT, coefficients );
   }


   double ParameterResolutions::interpolate( double eta, double coeffLow, double coeffHigh ) const
   {
      double slope = ( coeffHigh - coeffLow ) / ( m_etaHigh - m_etaLow );
      double value = coeffLow + slope * ( eta - m_etaLow );
      return value;
   }

   double ParameterResolutions::calculateResolution( double pT, const std::vector<double>& coefficients ) const
   {
      if ( pT == 0 ) return 0;
      double resolution = 0;
      std::vector<double>::const_iterator powerSeries = m_powerSeries.begin();
      std::vector<double>::const_iterator iter = coefficients.begin();
      std::vector<double>::const_iterator end  = coefficients.end();

      for ( ; iter != end; ++iter )
      {
         resolution += (*iter) / std::pow( pT, (*powerSeries) );
         ++powerSeries;
      }
      return resolution;
   }

}
