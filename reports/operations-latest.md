## 1. 작업 목표
64개체미충족중재등반만부족한14개체의접근/정렬전환에서국소수정후보를정한다.새게임반복없이기존기록을우선사용했다.

## 2. 실제 수행
닫힌approach-sixtyfour-on1의170초이후표본과입구선택/MoveTo/Follow/Crowd코드를대조했다.입구근처복구이동간섭가설을먼저검토했다.새default-OFF nf_entry_facing 모듈을별도로생성·빌드·서명했다.기존pose/hip모듈과원본은바꾸지않았다.

## 3. 측정 결과
14개체표본에서LINK_ALIGN→ALIGN_HEADING전환156개,목표동일156개,선택연결동일147개.고정groundCLIMB입구평면의normal오차0.065m밴드교차78개.인접표본사이에여러callback이있으므로정확한전환횟수·단일callback인과로확대하지않는다.같은집단입구근처retained pose recovery활성표본은0으로복구억제가설은지지되지않았다.원래64기준46/64실패는그대로다.새실행0,p50/p95/p99/화면/CPU/GPU/GC미측정,20ms목표미달성이다.

## 4. 원인 분석
사실:정밀setback이벗어나면Follow가일반Walk를호출하고MoveTo는경유점방향으로몸을돌린다.다시정렬준비가되면Follow가입구방향으로돌린다.군집Displace는heading을직접바꾸지않으며FaceEvidence는Active연결중회전을막고있어해당기존수정을중복하지않는다.

가설:밀침등으로짧은위치보정이필요할때두heading소유경로를반복하는것이재등반지연에기여한다.직접heading이없어확정은아니다.후보는EntryAligning groundCLIMB에서Admission.Entry와동일한0.36m이내보정목표에한해몸방향을유지하고원본planar root이동량을기존경유점방향으로보낸다.기존경로계산/월드/발/개체/플레이어검사와Body.SolveContacts는유지한다.허용각·입구오차·기한·밀침·낙하검사를완화하지않는다.원본위치로snap하지않는다.

## 5. 검증 수준
표본/분기진단부분성공.빌드·자동서명·DSCheckSignatures·packed script대조성공.원본DGBGroundAgent는실제로로딩한dg_lab_inputs와내용해시일치.후보MoveTo의next=Position+step이후물리검사본문은원본과동일하다.실제엔진컴파일·물리fixture·8/64·시각·성능·최종수용미실행.

## 6. 변경 사항
새nf_entry_facing만생성했다.플래그없음/비native/범위외는원래MoveTo로위임한다.전투/사망/목표변경/우회/양보/정렬전/다른영역은새Walk잠금에서제외한다.스코프내에서는root의수평길이와남은경유점거리로step을제한한다.짧은보정에서발미끄러짐·밀침에진전불가·회복진동이위험이다.원본/기존배포/Steam/운영서버/자산미변경,개인키미포함.빌드ID는공개JSON에연결한다.

## 7. 원시 증거
reentry-recovery-ownership-audit와reentry-facing-switch-audit,원본gameplay해시,후보plan/source/build/signature/packed/loaded-source-equivalence를로컬체크포인트로연결한다.처음GroundAgent를integration에서찾은검색은없음을확인한뒤실제dg_lab_inputs로바로잡았다.공개는집계·판단이며원시전체/개인경로/자산은비공개로보존한다.

## 8. 잔여 문제
다음한정fixture에서플래그OFF/범위외동일성과잠금중heading유지·native이동량상한·벽/턱/개체/플레이어차단을검증한다.안전진전이없거나차단누락이면기각하고8/64반복하지않는다.통과시에만원래8→64기준을사용한다.회전속도증가/충돌삭제/복구억제/기한연장은하지않는다.복구활성간섭패치는현기록의지지가없어작성하지않았다.전체골미완료.

로컬checkpointSHA256: d674635bb87f623d28a975bab8610de2b356df02ca08d06769c1543924820e72
