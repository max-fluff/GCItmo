#include "DirectionalLight.h"


void DirectionalLight::GenerateOrthoMatrix(float width, float depthPlane, float nearPlane)
{
   orthoMatrix= DirectX::XMMatrixOrthographicLH(width,width,nearPlane,depthPlane);
}

void DirectionalLight::GenerateViewMatrix()
{
   DirectX::SimpleMath::Vector4 lookAt=DirectX::SimpleMath::Vector4(0,0,0,1.0f);
   DirectX::SimpleMath::Vector4 up = DirectX::SimpleMath::Vector4(0.0f,1.0f,0.0f,0.0f);
   viewMatrix=DirectX::XMMatrixLookAtLH(pos,lookAt,up);
   
}

void DirectionalLight::GenerateOrthoFromFrustum(const Matrix& view, const Matrix proj)
{
    std::vector<Vector4> frustumCorners=GetFrustumCorners(view, proj);
    Vector3 center =Vector3::Zero;

   for (const auto& v:frustumCorners)
   {
      center +=Vector3(v);
   }

   center/= frustumCorners.size();

   viewMatrix=DirectX::XMMatrixLookAtLH(center,center-Vector3(direction),Vector3::Up);

   float minX = 10000000.0f;
   float maxX = -10000000.0f;
   float minY = 10000000.0f;
   float maxY = -10000000.0f;
   float minZ = 10000000.0f;
   float maxZ = -10000000.0f;

   for (const auto& v : frustumCorners)
   {
      const auto trf = Vector4::Transform(v,viewMatrix);

      minX= min(minX,trf.x);
      maxX= max(maxX,trf.x);
      minY= min(minY,trf.y);
      maxY= max(maxY,trf.y);
      minZ= min(minZ,trf.z);
      maxZ= max(maxZ,trf.z);
   }

   constexpr float zMult = 10.0f;
   minZ = (minZ<0) ? minZ*zMult : minZ /zMult;
   maxZ = (maxZ<0) ? maxZ/zMult : maxZ * zMult;


   orthoMatrix=Matrix::CreateOrthographicOffCenter(minX,maxX,minY,maxY,minZ,maxZ);
   
}

std::vector<Vector4> DirectionalLight::GetFrustumCorners(const Matrix& view, const Matrix proj)
{
   const auto viewProj = view * proj;
   const auto inv = viewProj.Invert();

   std::vector<Vector4> frustumCorners;
   frustumCorners.reserve(8);
   for (unsigned int x = 0; x<2; ++x)
      for (unsigned int y=0; y<2; ++y)
         for (unsigned int z=0; z<2; ++z)
         {
            const Vector4 pt = Vector4::Transform(Vector4
                (
                    2.0f*x -1.0f,
                    2.0f*y -1.0f,
                    z,
                    1.0f),inv);
            frustumCorners.push_back(pt/pt.w);
         }


   return frustumCorners;
}

std::vector<Matrix> DirectionalLight::GenerateOrthosFromFrustum(const Matrix& view, const Matrix proj,float farZ)
{
   std::vector<Vector4> frustumCorners=GetFrustumCorners(view,proj);
   orthoMatrices.clear();
   viewMatrices.clear();
   distances.clear();
   
   int n=4;
   int exp=2;

   

   for (int i=1;i<=n;i++)
   {
      std::vector<Vector4> newCorners=std::vector<Vector4>();
      for (int j=0;j<8;j+=2)
      {
         newCorners.push_back(frustumCorners[j]+frustumCorners[j+1]*pow((i-1+0.0001f)/n,exp));
         newCorners[j].w=1.0f;

         newCorners.push_back( frustumCorners[j]+frustumCorners[j+1]*pow((i+0.0001f)/n,exp));
         newCorners[j+1].w=1.0f;
      }

      Vector3 center =Vector3::Zero;

      for (const auto& v:newCorners)
      {
         center +=Vector3(v);
      }


      center/= newCorners.size();

      Matrix viewMatrix2=DirectX::XMMatrixLookAtLH(center,center-Vector3(direction),Vector3::Up);

      float minX = 10000000.0f;
      float maxX = -10000000.0f;
      float minY = 10000000.0f;
      float maxY = -10000000.0f;
      float minZ = 10000000.0f;
      float maxZ = -10000000.0f;

      for (const auto& v : newCorners)
      {
         const auto trf = Vector4::Transform(v,viewMatrix2);

         minX= min(minX,trf.x);
         maxX= max(maxX,trf.x);
         minY= min(minY,trf.y);
         maxY= max(maxY,trf.y);
         minZ= min(minZ,trf.z);
         maxZ= max(maxZ,trf.z);
      }

      constexpr float zMult = 10.0f;
      minZ = (minZ<0) ? minZ*zMult : minZ /zMult;
      maxZ = (maxZ<0) ? maxZ/zMult : maxZ * zMult;


      orthoMatrices.push_back(Matrix::CreateOrthographicOffCenter(minX,maxX,minY,maxY,minZ,maxZ));
      viewMatrices.push_back(viewMatrix2);
      distances.push_back(Vector4(farZ*pow((i*1.0f/n),exp),
         farZ*pow((i*1.0f/n),exp),farZ*pow((i*1.0f/n),exp),1.0f));
   }
   
   return std::vector<Matrix>();
}
