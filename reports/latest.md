# 5파트 Run 전체 몸체 실제 표시

작성: 2026-11-05T04:53:52.566108+09:00

## 1. 작업 목표
5파트401본Run 후보의실제전체몸체표시와관찰구간접속유지를확인한다.기존BattlEye재시작안내를자산오류로단정하지않는다.
## 2. 실제 수행
기존release loopback fixture에참조5파트/후보5파트를배치했다.Steam applaunch에기존런처가인식하는useBE옵션을전달했고실제DayZ_BE→일반client생성을확인했다.보안설정·운영서버변경없음.60초관찰과두시점화면저장후client창정상닫기.
## 3. 측정 결과
10타입생성및60초Run/retirement DONE확인.두전체몸체가다른Run포즈로표시되는두snapshot확보.이구간Game restart required(400F0)미관찰.서버완료후강제종료에따른연결종료40004관찰.client종료성공RPT표식및process부재확인.exitcode미보유.프레임시간/CPU/64기능미측정.
## 4. 원인 분석
실제5파트몸체표시·포즈변화제한관찰성공.이번bootstrap경로에서재시작안내없었으나단일시험으로useBE가유일원인을해결했다고확정못함.두snapshot은프레임별부드러움·모든면·seam·skinning동등성증명아님.
## 5. 검증 수준
5파트전체몸체Run표시:제한진단성공1회.지속부드러움/14클립/8·64기능/CPU성능/최종수용:미검증.단일성공을반복검증으로기록하지않음.
## 6. 변경 사항
기존서명검증된fiveRunPBO재사용,새PBO수정없음.격리mission/clientprofile/launch조건만추가.기존8파트운영본보존.실제게임play어추적·등반·낙하검사는이번시험에없음.
## 7. 원시 증거
palette64-five-visible1/plan.json,steam-launch-request.json,server-launch-BProbe.json,visual-analysis.json,role-pose.tsv및clientRPT.두jpg는비공개로보존하며공개에는fileID/SHA/관찰한계만제공.
## 8. 잔여 문제
다음은이5파트Run구조와기존8파트Run구조의같은정지몸체조건에서전체비용차이를분리비교할최소실험준비.실제gameplay승격전에14클립/물리·낙하보존검증이별도로필요.20ms목표미달성.공개미러는로컬묶음저장후갱신한다.

