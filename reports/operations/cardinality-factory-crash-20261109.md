# 골격 진단 기준 factory 실패 분리 — 현재 참조 생성 경로 확보

## 1. 작업 목표
48→248 P3D골격목록통제쌍의공통CPU포즈/실제화면을확인한다. 원래NPC이동·충돌·물리시계는변경하지않고,기준생성문제와후보표시실패를구분한다.

## 2. 실제 수행
기존server/client/고정camera/관측pattern을재사용한2actor Run미션과default-OFF client48본reader를준비했다. 관측PBO를별도빌드/서명검증했다. 서버compile-only실행은entity0으로World/Mission컴파일완료후종료했다.
네트워크시도는기준Animal_DGNativeRepartition0 factory경로에서서버접근위반종료,client는연결상실메뉴로돌아왔다. pair ready/pose표본생성전이므로유효비교0이다. 소유client를정체성확인후CloseMainWindow로닫았다.
후보/관측PBO를모두뺀기존10PBO server-only에서옛기준클래스하나만생성하도록분리했다. 같은접근위반이재현됐다. 이후같은fullnative XOB/instance설정을쓰는현재64본모델참조클래스Animal_NFPaletteFull_reference0를기존PBO만으로생성해반환·명령바인딩·퇴역을확인했다. 새모델을다시만들거나클라이언트를다시실행하지않았다.
기존AMD64 minidump reader와해시고정PE/Capstone를재사용해두덤프의faultRVA/바이트를비교하고해당246byte unwind범위만정적으로읽었다. Ghidra전체초기화/후킹/네이티브쓰기없음.

## 3. 측정 결과
| 실행 | 후보/관측PBO | 실제결과 |
|---|---|---|
| compile1 |포함|entity0,World/Mission컴파일완료,exit−1,noforce|
| network1 |포함|factory Spawn CreateObjectEx위치접근위반,exit−1073741819|
| control-factory1 |둘다제외|기존48본class생성단독접근위반,exit−1073741819|
| known-reference-factory1 |둘다제외|현재64본/fullnative참조class반환nonnull,RequestedState11/release12/landing13명령바인딩,detached=true,exit−1|
두crash는같은server987fbc47…에서fault RVA0xaa9859/같은명령바이트. fault명령은null RAX기준+0x54읽기이며예외read주소0x54와일치한다. 객체/필드의정확한native의미는미확정이다.
유효CPU포즈/active비교프레임0,실제시각판정없음.8/64기능·CPU/Present p50/p95/p99미측정. 관측PBO SHA256 `306933CC51FDA3853D0AD480340A082CF4B22302BA4E5AB0AB162683445C5453`,서명검증성공. 정적모델후보E6630AE0…의기존서명검증은유지한다.

## 4. 원인 분석
확인: 새로운후보를제외해도옛기준factory가실패한다. 두faultRVA동일은같은native실패지점을뒷받침하지만완전한원인복구는아니다. 이결과를248골격의표시실패/성능회귀로기록하면안된다.
확인: 현재64본모델/fullnative참조class는생성·명령바인딩·안전퇴역이가능하다. 따라서full XOB/native_combat인스턴스가현재엔진에서항상깨졌다는일반화도반증된다. 현재참조와옛48본class는model/상속차이가있으므로정확한단일원인은미확정이다.
판단: 옛48본기준factory복구를별도기능개발로확대하지않는다. 현재작동한64본참조를대조군으로설계를정정하고,같은시각면/Memory/物理LOD바이트를유지하는64→248입력을다시정의하는것이더직접적이다. 새입력은아직생성하지않았다.

## 5. 검증 수준
관측module컴파일·서명검증통과. 네트워크쌍진단실패/유효표본0. 후보없는옛기준factory실패재현완료. 현재fullnative참조factory진단성공. 후보render/CPUpose/성능/8→64회귀/최종수용미완료.20ms p95목표미달성유지.

## 6. 변경 사항
cardinality-probe1/runtime1 안의고정미션/작은default-OFF관측PBO/기존실행기재사용/읽기전용덤프파생물만추가했다. 기존모델·PBO·Script·이동·지지·충돌·운영설정불변. 실행모든입력PBO해시후검사일치. 서버4개모두forced=false,client소유종료확인. 크래시를정상퇴역으로보고하지않는다. 개인키미포함.

## 7. 원시 증거
ANIM-cardinality-compile1/network1/control-factory1/known-reference-factory1. 각plan/launch/terminal/script log,network및control의crash log/minidump/reader결과,통합factory-crash-comparison1.json,factory-fault-static1.json,observer빌드/서명영수증. 원시실패를덮어쓰거나비교실행성공으로소급하지않았다. 공개에는덤프·원시RPT·플레이어ID·로컬경로·프로세스/절대주소를포함하지않는다.

## 8. 잔여 문제
다음단일산출물은작동확인된현재64본/fullnative참조class기반의64→248평면P3D골격입력계약이다. 정적LOD/가중치/geometry/Memory/기준위치동등을다시검증하고서명후,소수고정쌍에만복귀한다. 현재네트워크실패를고치려고원래NPC기능/시계를수정하지않는다. 한쌍진단전14클립/전체합체/8→64성능확대없음.
추가사용자답변:33ms격리비교허용을받았지만이후확인한20Hz native콜백근거로이미철회한후보다.50~100ms지연이나다른물리오차허용으로확대하지않고구현하지않았다.
