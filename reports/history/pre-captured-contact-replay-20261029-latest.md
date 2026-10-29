# 실제 접촉 이력 확보: 8·64개체 기록과 착지 거부 원인

실제 접촉 이력 확보 — 8→64 기록 연결과 착지 거부 분리

작성: 2026-10-29T19:48:18.541661+09:00

1. 작업 목표

기존 고정 포즈 재현에서 빠졌던 실제 이전 Contacts4점과 같은 Fall 호출의dt/수직 및압력속도를 확보한다. 착지 질의 허용과 실제 LAND 전환을 구분하고,기존 정상 낙하/복구·추적 동작을 보존한다. NPC 이동/애니메이션 방식이나 성능 최적화는 변경하지 않는다.

2. 실제 수행

기존F6DE 빌드·원본9script를 먼저 보존했다. opt-in previous-contact-detail.flag 계측 파일1개를 추가했다. 원래 bounded LDObserveLanding이 실제로 행을 쓴 경우만 같은 sequence의companion 행을 쓴다. 실제 cached4world contacts·Fall dt/후중력속도·압력속도·root/heading/lift·native요청/clock·directorNow/Born/engineTickTime을 읽으며 추가 물리/본 query와Body 필드 쓰기는 없다. 기존9script는 바이트 동일하다.

PBO10개 script의source/packed 바이트,서명·검증을 통과했고,별도 엔진4case에서 flagOFF/실제Fall/빈 Contacts/중복 제한을 확인했다. 과거compile 기록을 덮어쓰지 않도록 실제 수집용 미션을 정확히 복제했다.8명300초,64명300초 혼합 전경,같은64명300초 전경 실행을 각각 정상 종료했다. 마지막64 실행은 CUA로 대상 DayZ 창만 활성화했고 다른 작업의 프로세스/공유 입력 런타임은 보존했다.

3. 측정 결과

|실행|개체|최종 도착|고지대 미도달|낙하 후 재개|접촉 기록 연결|미션p50/p95/p99 ms|
|---|---:|---:|---:|---:|---:|---|
|runtime-20261029-190552|8|8|0|9|10|22.0/33.0/38.0|
|runtime-20261029-191331|64|64|25|60|84|65.0/115.0/135.0|
|runtime-20261029-193252|64|64|27|55|106|52.0/149.0/195.0|

200/200 companion 행이 기존 LD행의sequence/id/generation/phase/root/heading/lift/native clock과 일치했다. 이는 샘플링된200행의연결이며 모든 physics 호출을 전수 기록했다는 뜻이 아니다. 엔진 proof는기존4행/companion3행이고 OFF1행은추가출력없음,빈1행은UNAVAILABLE,중복observer1행은억제됨을 확인했다. 실제8 dt는0.0159912~0.0410004초,p50 0.0209961초였다.64는최대/p50 0.05초였고 압력속도는0이 아닌 표본이 있었다. 기존0.05초/압력0 정규화 재현과 구분해야 한다.

owner는8명6회/혼합64명44회/전경64명40회의 VALIDATED_FOLLOW가 실제 전환과 연결됐고 위반은검출되지 않았다. 중간helper lease 실패는혼합64 시간2회,전경64 거리2회+시간1회였다. 이들은최종owner미종결과 다르며 모두최종인계로끝났다. CLIMB_EXIT_VALIDATION은이번기록에없다. 고지대 미도달25/27명은최종64/64로지우지 않는다.

혼합64의전경cadence266/3139와미션전경표본163개는전구간성능평가로불충분하다. 전경64는cadence4685/4685전경·미션표본4626개였다. 미션dt는GPU Present/표시애니메이션시간이아니고,다른승인작업과동시실행·상세로그·플레이어실이동거리차이가있다. 전경64의GROUND_EAST/NORTH 첫끝표본직선변위는약38.707/14.636m,8명은39.797/39.805m였다. p95149ms를이전181ms나고정자극219/241ms 대비최적화성공으로해석하지 않는다.20ms목표는미달이다.

전경64에서callback NativeSeconds 누적은관측294.904초에175.606초,비율0.59547이었다.8명비율0.78587이었다. 이값은script에누적된callback pDt이며direct graph phase나실제화면재생배속이아니다.8명초기장면만직접관찰했고64전경전환시로딩화면을확인했다. 연속영상/실제표시프레임측정은없다.

4. 원인 분석

관찰 사실: 혼합64의LandingSpace true66행은물리LAND60회와같지않았다.61개질의행이60개실제전환과호환됐고,동일위치/같은director시각의2개질의가1개전환에대응해호출단위는모호했다. 전경64는허용60행·실제LAND55회이고5개행은직접전환과연결되지않았다. true질의수를물리착지성공수로보고하지 않는다.

혼합64 id4/seq226과전경64 seq152/197/284는허용된발판지점에서관절0 SWEEP 거부기록과연결됐다.컨테이너 충돌의실제normal/fraction·anchor·nativeSeconds·캐시된previous joint가맞았고cached previous차이는0이었다.모델좌표에서재구성한raw와기록된raw의차이는약0.26~0.40mm였으며native bit동일성은아니다. 이4개는실제관절충돌의근거다.나머지미연결행은throttled failure상세가없어원인을확정하지 않는다.

중요한clock관계도확인했다. contact-failure seconds는절대Director.Now,LD seconds는Now-Born이었다. 새companion의directorNow/Born으로연결했으며예:id4는125.318-41.796=83.522다. 서로다른clock의seconds를그대로맞추지 않는다.

추정: 경사면/모서리의전신하강거부를실제입력으로재현한뒤,신선한native normal/fraction을사용하는제한된contact-plane slide를검토할수있다. 아직그후보는구현/시험/배포하지않았다. NativeSeconds비율과사용자가느낀저속재생의관계도표시영상없이확정할수없다. 원본Days Gone전체알고리즘을확보했다는의미가아니다.

5. 검증 수준

진단 성공: 읽기전용source/바이트·서명·엔진4case·실제200행same-call연결·clock변환·물리LAND와질의분리·4개관절거부연결.8명은이번정의된시나리오의부분회귀를통과했다.64명은최종64/64지만고지대25/27과helper실패가남아전체회귀미수용이다. 전경조건/실제자극이다른64실행은통제된반복최적화검증이아니다. 시각품질·20ms·최종수용은미완료다. 동일시각반복질의의정확한Fall호출식별도미완료다.

6. 변경 사항

subset_previous_contact_detail.c를DGSubsetZZPreviousContacts로추가했다. builder에복사1개,launcher에명시적flag/검증·격리미션선택,compiler에별도검증이름을추가했다. 기존9script·base453D·render0C6E는동일하며NPC실행/전투/애니메이션/보간/충돌gate를수정하지않았다.

새integration D61DE1231E4F0DE13FC9DC91097031BE8E2B1EACD85FBD894BB3475BE955116E는빌드·자동서명·서명검증·source/packed10개검증성공이다. 엔진compile두종과3개클라이언트시작전의서명검증도성공했다. build.json의engineCompiled=false는빌드시점보수적메타데이터로유지하고후속실제compile결과를별도보존했다. 개인키는배포하지않았다. 운영서버·이전PBO아카이브·raw를보존했다.

7. 원시 증거

세runtime의launch/compile/config·native-ground/player/state/traversal/pose·landing/previous/contact-failure/owner TSV와분석을연결했다. baseline-F6DE와candidate-D61D의release/source/build/member/signature를보존한다. engine-join-proof와별도previous-contact-proof/live compileprofile도보존했다. 2mm의초기관계분석과엄격좌표분석의모호성기록을덮어쓰지않았고최종outcome-audit에한계를명시했다. 공개패킷은선정표본·집계·새관측/분석함수로제한하며전체내부로그·PBO/게임자산·프로세스/로컬경로는노출하지않는다.

8. 잔여 문제

우선실제id4/seq226과전경3표본을actual4previousContacts/현재MSpose/dt/속도로재현해발판질의와전신SWEEP거부가맞는지검증한다. 그후에만한가지제한된contact-plane응답을검토하며기존root/관절/월드/peer검사와실제LAND조건을우회하지않는다. 정상낙하·밀침·재등반을보존한8→64실기검증이필요하다.

정확한 Fall-call serial/after-result가 없어 중복 질의1건의 호출 배정은 미확정이다. 전체 pre-Fall/peer 이력도 포함하지 않는다. 이는 next-experiment-plan에 남겼다. 고지대 미도달과 helper 거리/시간 한도, 전투 후 owner guard 복귀, CPU-표시 시간 join, 64명 비용 귀속/20ms는 별도 잔여 작업이다. 애니메이션 재생/업데이트 방법을 추측으로 바꾸지 않았다. 모든 실험은 정상 종료했고 골은 활성 상태다.

원본 보고서 ID: 2026-10-29_194818_541661+0900_npc-previous-contacts-capture.md
원본 SHA-256: 8cc510b8e1dec37e487e901247249a68583c866f292fb46a789877a438ca0b1c
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 함수·대표 기록: [검토 패킷](../downloads/checkpoints/2026-10-29_194818_541661+0900_npc-previous-contacts-capture_evidence.txt)

[이전 기능 분석](../downloads/admission-report.txt) · [이전 애니메이션 분석](../downloads/animation-report.txt)
