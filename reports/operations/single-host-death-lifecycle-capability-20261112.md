# 단일 호스트 사망 수명 진단 — 포즈 평가 지속 관찰, 종료 접근 위반으로 미수용

## 1. 작업 목표
미세 중복 Ray 후보의 낮은 대상 비중 이후, 현행 렌더1+피해수신체1의 호스트 비용을 줄일 구조적 후보의 선행 수명 계약을 확인한다. 호스트128→64는 구성 산술일 뿐CPU50%절감 예측이 아니다. 구현 전에 실제 NPC를 변경하지 않고 네이티브 사망 후 기존 script 평가 가능성을 판단한다.

## 2. 실제 수행
현재 single224 Initialize·DGNativeProbe EEHitBy/EEKilled/HandleNativeCommand·Combat FreezeDamageReceiver·DamageEntity 생존 스냅샷·시체 인벤토리 소유자를 읽었다. 수신체는 네이티브 생명/부위 피해를 맡고 렌더는 살아 있는 상태로 사망 클립을 재생한다. 두 참조를 alias하면 FreezeDamageReceiver가 표시 몸체를20m아래로 옮기고 충돌/활성을 끄므로 직접 통합은 계약상 기각했다.
기존 서명된 프로브/원본13PBO를 재사용한 격리3호스트 시험: 살아있는 script 대조, 네이티브 사망 렌더 역할, 네이티브 사망 수신체 guard 역할. 동일 현재모델·GroundBody없음·root적용없음.4초에 모두 PreviewRequest12, 뒤2개체만 SetHealth0;12초까지 callback·Age·Head/Hips CPU포즈를 기록했다. 실제 총기·부위 피해·FireGeometry 시험이 아니다.
첫 미션은 존재하지 않는 GetCurrentCommandID 조회로 컴파일 실패했다. 실패/강제 종료 영수증을 보존하고 별도 revision2에서 해당 조회만 제거했다. 서버 runtime compile은통과했으나 종료에 접근위반이 발생했다.

## 3. 측정 결과
실행 SINGLE-HOST-death-capability1-r2, 독립1회, 성능 비교쌍0회,570행/각190표본. 사망 후4.246~11.945게임초에서 사망2개체 IsAlive=false·Health0, script존재 표본100%, PreAnim/PrePhys 각각154회 증가, 자체Age7.7초 증가, NativeRequest12 유지. 대조는Health500·생존이다.
측정한Head MS포즈는 살아있는대조와사망렌더의동시표본에서최대차0m. 두대상모두해당사후창의첫Head로부터최대약1.540092m변화했다. 이는해당본의CPU평가관찰이며전신14클립·실제화면품질증명이아니다. 수신체 guard 역할도script평가가지속돼guard만으로즉시명령이끝난다는예상은지지되지않았다.
스크립트E오류0이지만네이티브서버종료코드−1073741819(0xC0000005). DG_ROLE_DONE과3개RETIRE_HANDOFF_DEFAULT표식뒤crash,8초후DETACHED표식없음. 따라서정상종료/수명gate실패다. 클라이언트는정상닫기·강제종료없음. 현재모든시험프로세스소멸확인.
CPU·Present p50/p95/p99·실제시각품질·8/64전체NPC·부위피해·전리품미측정. 사망후평가관찰을최적화성과로보고하지않는다.

## 4. 원인 분석
확인: 네이티브체력0이어도기존script명령과해당Head포즈평가는사망후약8초동안계속될수있었다. 네이티브사망이즉시모든script평가를중단한다는가정은이실행에서성립하지않았다.
확인: 종료/명령퇴역전후네이티브접근위반발생. RPT의script stack은Finish32행(상태설정·파일닫기동일행),로그에는그뒤의퇴역handoff표식도있다. 정확한C++원인·어느개체소유권파괴가잘못됐는지는모른다. 단순히CloseFile나죽은개체퇴역이원인이라고확정하지않는다.
모름: 실제피격형상을통합해도native클립/건강6부위·시체·퇴역이안전한지,호스트별CPU기여. 기존27본수신체HeadRay불일치와같은자산위험도남는다. 포즈부분성공으로이를우회하지않는다.

## 5. 검증 수준
사망후기존script·특정본포즈지속:부분진단성공. 정상종료/수명:실패. 전체통합·반복성능·시각품질·8→64회귀·최종수용:미실행/미수용. 초기analysis.json의연속성판정은이관찰에만해당하며analysis-disposition.json이전체미수용·crash를명시한다.20ms p95는목표다.

## 6. 변경 사항
single-host-death-capability1 내격리미션·계약·로그·분석/판정만추가. 게임코드/PBO/모델/그래프/운영서버/원시기록보존. 기존프로브PBO SHA256 5d7184ef91b44ff4a4a5ffb0ccca918cf20b46a75324d24af78cabc49f651db8 및기존서명·검증증거재사용. 새PBO빌드/재서명없음. runtime중강제종료없음이나server native crash로정상종료아님. 자동생성minidump는기존실험폴더에남기고복사·추가덤프생성·공개하지않는다.

## 7. 원시 증거
contract.json, 미션init.c, single-host-death.tsv570행, server-terminal/client-closed, script/RPT/crash로그, analysis.json, analysis-disposition.json, 상위PRE_IMPLEMENTATION_REVIEW.md 및첫실패failed-terminal.json을연결한다. crash로그SHA256 49cea9b63b7b13d5917dc55feccf4379b18712cae6989827bdd13dbeae9f2ef2. 공개는집계·선정문맥·해시만제공하고설치자산·원시덤프·키·프로세스정보·로컬절대경로제외. 원시및기존보고서덮어쓰기없음.

## 8. 잔여 문제
안전수명이입증되지않았으므로현재호스트통합을구현하거나64로확대하지않는다. 이실패를연쇄사망기능/퇴역수정개발로전환하지않고보존한다. 다음후보판단에서는이미실패한native벽대체·낮은Ray중복·Scratch·SupportSweepFast·Drive/Displace단일끝점병합을재실행하지않는다.
현재안전하게입증된큰폭CPU절감후보는아직없다. 많은질의를실제로줄일수있는조건과기능안전계약이확보돼야하며,가장쉬운작은함수의좋은퍼센트나회귀도착률만으로목표달성을대체하지않는다. 기존단일렌더+수신체의검증된기능·사용자시각관찰을보존한다.
