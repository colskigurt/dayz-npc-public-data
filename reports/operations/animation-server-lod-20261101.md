# 생성시LOD0 통제시험:안정200ms 유지·후속50→200ms 덮어쓰기 관찰

작성: 2026-11-01T15:27:38.044503+09:00

## 1. 작업 목표

동일한1파트 물리 리더에서200ms 갱신이 공개API의 생성 직후LOD0 지정으로 줄어드는지 통제 비교한다. 효과가 없으면 실제 호출·후속 필드 변화를 확인해 단순미호출과정책 덮어쓰기를구분한다. 클라이언트·기존NPC 파이프라인은 보존한다.

## 2. 실제 수행

같은453D/0C6E/B8FB,서버EE1A,1파트DGSubsetIntegratedCrowdLeader,고정run/root이동OFF/플레이어없음/loopback/50FPS제한으로A1/B1/B2/A2를진행했다. A는자동LOD,B는생성직후 UpdateSimulationPrecision(0) 한줄추가다. 두init의해시와그한줄을제거하면동일바이트가됨을검증했다. 각실행에서기존3PBO서명을확인했다.

네 통제 실행에는native훅이없다. 공통실행기에서READY/DONE을polling한프로세스전체CPU counter를수집했다. 별도BProbe1회만기존PythonFrida관찰기를실제생성전에연결해공개setter전후와같은entity평가의필드를읽었다. 수치·모드·포인터를직접쓰지않았고12초관찰뒤훅을끄고소유서버종료까지유지했다. probe는비용비교에서제외했다.

네실행의공통Mission모듈CRC가같다는사실을동적init변경의노출증거로사용하지않았다. source해시·실행경로와동일B소스의실제native호출을연결했다. 설치서버/client scripts.pbo가동일바이트임을확인하고3056script멤버의API이름을검색했다.

## 3. 측정 결과

네통제실행과probe모두DONE·원시기록저장뒤소유서버종료를확인했다. 정상종료라고기록하지않고계획된Stop-Process를명시했다. crash0이다. 안정측정은spawn후처음2초를제외한약25초이며시작부일시효과를전체무효로확대하지않는다.

자동LOD2회와LOD0지정2회모두command누적시간증분p50/p95/p99는약200/200/200ms였다. median차이0ms/0%다. 기록된CPU Head좌표반복비율은A90.0000/90.0080%,B90.0000/89.9920%로차이는약-0.008percentage point이며표본끝점1쌍수준이다. 시각적개선으로판정하지않는다.

전체서버processCPU는A88.126/90.026ms/벽시계초,B76.483/89.286ms/벽시계초다. 평균차이는-6.191ms/초(-6.951%)지만범위가겹치고각2회뿐이다.0.5초poll 경계오차와엔진전체작업을포함해원인별CPU selftime이아니며최적화성공으로수용하지않는다. mission입력dt도CPU/Present frame-time이아니다.

별도probe에서LOD0 setter호출1회,평가60표본,오류0을확인했다. 호출직전/직후requested50/50ms,actual50/200ms로같았다. 즉호출은포착됐지만값을바꾸는효과를before/after equality만으로증명하지않는다. 약173ms뒤첫평가에서는requested200/200ms,actual200/500ms가됐고60표본모두그값을보였다. mode는1이다.173ms는Date.now기반관측순서이며정밀frame계측이아니다.

전체DONE의update수는A1=134/B1=134/B2=164/A2=134/probe=134다. B2의초기구간차이를숨기지않으며안정25초의동일200ms와구분한다. 동일B소스의probe한번으로모든B의173ms전환시각을일반화하지않는다.

기본scripts의검색결과는선언과수동debugUI호출이었다. native정책·조건부컴파일·다른addon자동호출전체를배제한것은아니다. 새8/64회귀·GPU/Present·native graph clock·할당/GC·화면품질은미측정,20ms p95는목표다.

## 4. 원인 분석

확인: 이fixture에서생성직후공개LOD0호출한번은안정갱신주기를유지하는해결책이아니었다. 실제호출당시50ms인필드가첫평가까지200ms로다시쓰였다. 따라서단순히LOD0한줄을추가하는후속수정은수용하지않는다.

추정: 이후관측된200/500ms는기존LOD2표값과일치해자동LOD정책의영향과양립한다. 하지만어느writer/caller가이를선택했는지는아직포착하지못했으므로거리·가시성·AI정책중하나로단정하지않는다. 반복강제호출로정책과경합시키는수정도현재근거로진행하지않는다.

기존오프라인mode0의잔여시간리셋·CPU/화면연결과이번headless mode1의생성후정책덮어쓰기는다른관측이다. 이번연구를기존시계문제를고친결과로삼지않는다.

## 5. 검증 수준

고정조건A/B/B/A의안정cadence무효과와후속필드전환관찰은진단성공이다. 생성시LOD0한번을유지해결책으로쓰는가설은이번범위에서효과부족으로기각한다. API일반무효·모든실행역할무효는주장하지않는다.8/64회귀·시각품질·성능성공·최종수용은미완료다.

## 6. 변경 사항

격리research의작은fixture/공통실행기/분석기/5개profile만추가했다. 게임코드·PBO·ANM·운영서버·기존9350후보·개인키는보존했다. 기존3PBO서명·해시검증성공,새PBO가없어새서명대상은없다. 영상·새대용량덤프는생성하지않았다.

## 7. 원시 증거

animation-server-lod-20261101-1513. plan/results/exposure-summary/policy-source-audit/server-source-audit/precision-observer,실제source와각launch/TSV/RPT를연결한다. native관찰은PythonFrida결과이며REA런타임Evidence가아니다. 공개는집계·선정호출전후/평가표본·정적API문맥이며PID·ASLR base·실제CLI·전체원시로그는제외한다.모든소유서버와관찰기종료뒤해시·JSON·파일안정성을검증한다.

## 8. 잔여 문제

다음은기존1파트서버에서관측된50→200ms전환을발생시키는interval setter/LOD dispatcher의caller를수치변경없이포착하는것이다. 성공하면지원되는정책제어접점과현재API의적용시점을판정하고,못잡으면추측으로고정값을반복쓰기하지않고소유자경계를명시한다. 필요한자원은현재서버/캐시된관찰기이며클라이언트/영상은이단계에불필요하다.

해시/서명불일치·crash·observer오류·범위밖entity가나오면후속실행을중단하고기록을보존한다. native graph/표시측정이없으면CPU/command까지만확인됐다고명시한다.64기능/비용·오프라인표시진단은별도목표로유지한다.

선정 공개 본문. 로컬 보고서 SHA-256: `fd8f892d35fe05c45b449044e724eaf6fd22c5839cec7ceeec36115f7f2a91a6`. 원시 전체가 아니다.
