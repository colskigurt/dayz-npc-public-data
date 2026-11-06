# 상태제한SupportFast 이력 대조·5파트 의존성 어댑터 준비

작성: 2026-11-06T10:25:03.872538+09:00

## 1. 작업 목표
기존 상태 제한 SupportSweepFast의 구현/성공/실패 이력과 현재5파트 조합의 차이를 확인하고,같은 알고리즘을 새로 구현하거나 이미 끝난 시험을 반복하지 않는다. 정상 밀침·낙하·착지·FOLLOW·재등반과 기존5파트 연구 기준을 보존한다.

## 2. 실제 수행
과거 current-support-cost-review/support-context-review/slot-support-four-review1과 실제 후보 source/PBO/서명/계획을 읽었다. 후보 fast 메서드의 이름과 FAST 옵션을 역변환하면 현재 SolveContactsInternal과 정확히 일치했다. 이전 계획의 baseMethod SHA d7eefcd5…는 이번 추출에 마지막LF를 더하면 일치해,해시 차이가 코드변경이 아님을 확인했다. 현재 실행11개 PBO의 스크립트를 읽어 해당 원본 메서드 소유자가 dg_lab_inputs임을 확인했다.
기존 PBO의 script/source 일치·기존서명을 재검증했다. 그대로 재사용하려 했으나 requiredAddons의 NFContactEndData가 현재 조합에 없어 직접 로딩하지 않았다. 과거 모듈을 가져오지 않고 CfgPatches/CfgMods/패키지 prefix와 requiredAddons만 NFPaletteFiveGameplayData로 바꾼 격리 어댑터를 빌드·서명·검증했다. 스크립트 바이트는 그대로다. 현재5파트8명 OFF/ON 임무도 기존 자극/판정/타이머를 보존해 준비·사전 검증했다. 아직게임/임무실행은없다.

## 3. 측정 결과
| 구분 | 기존 근거/현재 판정 |
|---|---|
| 전역Fast 국소비용 |기존2독립실행 roof약26%절감·terrain약6.6%회귀,전역gate실패|
| 상태제한 guard |기존19/19진단통과,합성힌트시험|
| 상태제한 단독8/64 |각1실행8/8·64/64,낙하8/8·82/82복귀|
| 이후 slot+support64 비교 |OFF58/64·48/64,ON39/64·45/64,4/4기능실패·서버dt개선방향불일치|
| 현재5파트호환 |정적 method역변환동일·script바이트동일·새의존성어댑터서명검증성공|
| 현재5파트실제8/64·비용 |미실행/미측정|
옛slot+support의명령시간/미션시간 약0.61~0.64와실패는보존한다. 이를현재5파트의비용/기능 결과로바꾸거나파트수·시계차이가원인이라고확정하지않는다. 기존국소ticks를전체CPU/Present개선률로환산하지않는다. 이번새p50/p95/p99/시각/비용측정은없다.

## 4. 원인 분석
확인:상태제한설계는새아이디어가아니며이미구현·시험됐다. 지형overhead를피하고등록구조물FOLLOW에만기존실물검사를시도하도록한후보다. 현재메서드가같아solver재이식은불필요하지만,과거의존성을그대로추가하면현재조합에불필요한로직을섞을수있다. 새어댑터는이를피하는설정변경뿐이다.
확인:상태힌트는GroundPlatform/FOLLOW/low-high/fixture영역일치/active-dead combat부재이고,실제중앙ray·box·sphere sweep·법선/높이·원래joint/fallback검사는유지한다. guard불통과는원래경로로간다. 상태라벨을지지true로대체하지않는다.
한계:보수적lift는최대약8mm차이를허용해경로/접촉/복구이력을바꿀수있다. pending-hit/death·전투복합상태는옛19가드로수용되지않았다. 옛OFF도실패했다고해서ON원인가능성을없애지않고,옛단일64통과를반복안정성으로승격하지않는다. 새5파트조합의wrapper overhead와실제전체CPU효과는미확인이다.

## 5. 검증 수준
이력/중복확인·소스역변환·패킹바이트·의존성분리:진단성공. 새어댑터빌드/서명:통과. CfgConvert는설정문법검사이며Enforce/runtime통과가아니다. 새8/64·실제비용·시각·전투·최종수용은미실행/미완료다. 현재기준은격리연구프로필의5파트이며운영배포승격을뜻하지않는다.

## 6. 변경 사항
새 nf_five_support_context PBO SHA256 1419E96D11A984951CDD48B926D7E6604413F1B9D74729D8FE55118BAF86E2BC,DSCheckSignatures 성공·개인키미포함. script SHA256 233eaa91ec83e63f97dddeb5c7184470c41330d3e2084e3b206f14669da5ae56은원래패킹된후보와동일하다. 기존 PBO/solver/원본/애니메이션·물리/운영서버/공통실행기는불변이다. 새프로필의기본값은OFF이며옛후보를같이로딩하지않는다.

## 7. 원시 증거
support-context-five-review1/review.json·normalized-method.patch,현재폴더staging/build/signature/packed-proof/base-method-recipe-proof,eight1/staged-plan 및기존support-context-candidate1/guards1/eight-support-context1/sixtyfour-support-context1/slot-support-four-review1을연결한다. 기존원시/실패보고서는보존한다. 공개는선정집계·자체설정차이·해시이며전체모델/바이너리/원시로그/키/프로세스정보/로컬경로는공개하지않는다. 저장전실제게임/빌드프로세스종료·파일해시·JSON을검증한다.

## 8. 잔여 문제
다음은준비된동일5파트8명OFF/ON각1회의통합기능비교다. 기존19가드나192000회국소method비교를다시만들지않는다. ON의실제context사용·4ray/used 생략·상태계약·정상낙하/복귀·퇴역을확인한다. 새실패가나오면기준OFF와기존실패부터비교하고기능복구개발로확대하지않는다.64명전부가좁은지붕에동시점유해야한다는조건은추가하지않는다.
기능통과후에는기존process-cost도구의TotalProcessorTime 읽기와현재프레임수집기를재사용할수있는지판단한다. 과거고정active파일/출력경로는재사용하지않고시험별소유권·출력만연결해야한다. process전체CPU는함수self-time이아니며client프레임과별도판정한다. 실제전체이득의근거없이64명장시간회귀·새대규모설계로확대하지않는다.20ms/시각품질최종목표는남아있다.

