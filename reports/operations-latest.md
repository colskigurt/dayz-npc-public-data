## 1. 작업 목표

이미 구현된 SupportSweepFast의 비용을 현재8PBO 실행 조합에서 분리 비교한다. 지난 배열 재사용 후보는 약1.5~2.5%라 승격하지 않았다. 과거 Fast64는63/64·낙하 복귀95/96으로 실패했으므로 재설계 없이 이미 해결됐다고 취급하지 않는다. 이번은 기존 검사 옵션의 국소 비용 시험이며 게임 기능 수용 시험이 아니다.

## 2. 실제 수행

새 PBO 없이 mission이 기존 DGBNativeGroundBody.SolveContacts를 호출했다. REF는 기존10개 발 광선, FAST는 등록된 컨테이너의 중앙 ray·충돌box 내부 전체 footprint·실제 sphere sweep·법선/높이 일관성 가드를 통과한 발에만 주변4ray 대체, SHADOW는 원래 광선과 fast bound를 함께 비교한다. 낙하·밀침·피해·월드 sweep·원본 solver는 변경하지 않았다.

2독립 프로세스, terrain/roof/edge(offset1.2m)/minimumLift0.3 거부의4조건×4그룹×6블록, 각1000회 측정·32회 준비다. 총192블록·192000회, 팔별8블록/조건/프로세스다. 첫 REF/FAST/SHADOW/SHADOW/FAST/REF, 둘째 역순. 그룹 내 실제 idle4본 포즈와 root 불변 확인. 반환·reason·points 수 일치, shadow lift/points 동일, FAST lift는 REF 이상·차이8.001mm 이내, points가 해당 수직 차이만 갖는지 검사했다. 이8mm는 기존 guard 범위이지 일반 안전성 입증값이 아니다.

## 3. 측정 결과

|독립 실행|지형 FAST 대 REF|지붕|가장자리 offset|lift 거부|
|---|---:|---:|---:|---:|
|support-method-cost1d|6.6452%|-26.3039%|-14.7211%|6.1185%|
|support-method-cost2d|6.6206%|-26.4242%|-15.1073%|6.6650%|

두 실행에서 지붕 FAST는 측정 블록당2천 foot rays+4천 joint sweeps다(REF는1만+4천). 추가 StructureSupportBound sphere sweep은 fastUsed/attempt counters에 연결되며 jointSweeps 수에 포함되지 않는다. 지형 FAST는1만 foot rays를 그대로 수행해 절감0이다. 가장자리 offset은 한 발 적격 사례가 포함돼5개 ray/발 전체 생략이 아니며 FAST foot rays6천이다. 가장자리에서 무조건 원래10ray가 실행됐다고 하지 않는다. 실제 guard가 부적격인 발만 원래5ray로 돌아간다.

SHADOW에서 under counter0, 준비 계약 모두 통과. 전체 조건 p50/p95/p99는 JSON의 block raw ticks 분포로 공개한다. ticks는1000회 메서드 경과 카운터이며 CPU self-time·프레임시간이 아니다. 절대ms 환산하지 않는다. 반복5% 지붕 절감 조건은 충족했지만 지형 회귀3% 상한을 초과해 사전 전역 활성화 gate는 False다. 기각 기준을 바꾸어 통과시키지 않는다.

8/64 gameplay, Present·CPU busy·GPU·시각 cadence·할당량·GC는 이번에 미측정이다. 과거 matched-flat65~69ms를 이 수치로 줄여 계산할 수 없다.20ms p95는 미달성 목표다.

## 4. 원인 분석

확인: 적격 지붕에서 주변 광선을 줄이는 방식은 현재 solver 국소 비용을 반복 감소시킨다. 지형에서는 최적화 가능 여부를 확인하는 분기/guard만 더해져 약6.6% 경과시간 증가가 반복됐다. 지붕상의26%를 NPC 전체/서버/클라이언트 개선률로 주장하지 않는다. 실제 군중의 적격 비율과 호출 분포는 다른 문제다.

과거 연구와 중복: ray 감소·bounded overestimate·8/64 기능은 이미 시험했다. 새 근거는 현재 모듈 조합의 REF/FAST/SHADOW 역순 독립 비교와 지형 비용 증가다. 옛63/64 실패를 현재 수치로 반증하거나 기존 bounded-lift64/64를 FAST 활성 기능 통과로 재사용하지 않는다.

준비 실패도 보존했다. 최초 Enforce subtraction 표현식과 긴 로그 수식에서 컴파일 실패했다. 후속c는 물리 trial 실행 전에 알려진 diagnostic plugin warning의 기대 경로가 상대 경로라 strict guard가 거부했다. d는 c와 mission bytes 동일, 기대 경로만 정확한 절대 경로로 바꿔 원래 guard를 유지했다. 두 d 실행은 퇴역 handoff까지 완료했다. exit-1 소유 서버 정리는 정상quit 인증이 아니다.

## 5. 검증 수준

현재 코드의 제한된 메서드 진단:성공. 두 독립 프로세스 반복 비교:완료. 전역 FAST 수용:기준 미통과. 상태 제한 후보 구현·8→64 회귀·시각 품질·전체 성능 목표·최종 수용:미실행/미완료. 골 active 유지.

## 6. 변경 사항

격리 mission/launcher·분석·보고서만 추가. 현재8개 PBO hash/서명은 매 실행 전 검증 성공. 새 PBO/재서명 없음, 기존 게임/운영서버/자산/시계/키 변경 없음. 자체 staging/analyzer가 추가됐고 원시 실패/기존 보고서는 덮어쓰지 않았다.

## 7. 원시 증거

sourceRunId:network-gameplay/current-support-method-cost. support-method-cost1/1b/1c의컴파일·strict guard 거부,1d/2d의plan·동일 mission·서명/launch·정확한22열 TSV·RPT/retirement 및분석 JSON을 로컬 체크포인트에 연결한다. 공개는 선정된 메서드 수치·자체 시험 mission, 전체 원시/영상/설치자산/프로세스/로컬경로/키는 제외한다. 조건내8블록을8독립 프로세스로 세지 않는다.

## 8. 잔여 문제

현재 최우선 단일 후보 명세: 전역 FAST 대신, 등록 구조물 위 FOLLOW라는 기존 개체 상태를 활성화 힌트로만 사용해 지형에서 후보 overhead를 피한다. 힌트가 참이어도 원래 중앙ray·box·sphere·법선/높이 가드는 모두 유지한다. 실제 지지·착지·발판 가장자리 판단을 상태 라벨로 대체하지 않는다. MOTION/FALL/LAND·피격/사망·미등록 구조물은 원래 경로가 우선이다.

예상 이점은 적격 지붕 질의 감소, 한계는 주요 평지 추격 비용을 직접 해결하지 못한다는 점이다. 위험은8mm 이하 보수적 lift 변화의 경로 의존성·재등반/혼잡·모듈 override 소유권이다. 별도 default-OFF 작은 solver override와 상태/포즈 가드 시험 후8→64 동일하네스, 마지막 전체 프레임 반복 비교가 필요하다. 새 후보가 현재 실험에 섞이지 않게 별도폴더/PBO로 준비하고 원래 Fast64 실패 재현을 보존한다. 이번 보고만으로 새 후보를 구현/배포했다고 하지 않는다.

로컬 checkpoint SHA256: 8f6abfeef8bcf031a2a10dd18b9734a0b018939c3ede020d5df252624cf804ab
