# MS API 경로 정정과 포즈 저장소 alias·캐시 상태 확인

작성: 2026-11-01T17:44:40.427193+09:00

## 1. 작업 목표

진단 query188/interpolated190 값의 일치가 독립적인 포즈 결과인지 같은 저장소 alias인지 구분하고, compressed transform과 GetBonePositionMS의 정확한 좌표·캐시 경로를 확인한다. 원인 확인 전 새 보간/업데이트 방식은 적용하지 않는다.

## 2. 실제 수행

설치 A500 exe의 GetBonePositionMS 등록 문자열에서 실제 handler와 virtual dispatch를 다시 추적했다. 첫 재사용 생산자는 target override가 적용되지 않아 C670 해시를 반환했다. 이를 진단 근거로 쓰지 않고 보존한 뒤, A500 고정 생산자를 별도로 실행했다. C670 결과는 두 빌드의 slot 차이 비교에만 사용했다.

A500 handler RVA6E3240→virtual slot618→Animal C67B10→helper C04840→matrix copy C06C50→cache builder C10FF0을 확인했다. 이전 같은-slot 후보610에 붙였던 진단 MS 조회 명칭은 철회한다.610의 원시 명령은 유효하지만 진단 MS API binding은 아니었다. space-paths의 C06520 후보는 이 증명 경로에서 사용하지 않는다.

이후 동일1파트·고정run·root/AI 이동OFF·진단 fixture에서15초 읽기 중심 hook 관찰을1회 진행했다. 이번에는 CU 활성화와 readiness marker를 계측 전에 연결했다. 매 scheduler 종료에 pointer equality, compressed pose, 기존 matrix cache와 dirty flag를 기록했다. API/cache builder를 직접 호출해 결과를 갱신하지 않았다. 계측 중 실제 모델 화면1장도 확보했다.

## 3. 측정 결과

15.4938916초 범위·772표본,4경로 각각772회, 계측 오류0·script 관찰 오류0이다. scheduler-exit callback 간격 p50/p95/p99는19.986/22.941/24.528ms다. 실제 game frame-time·display interval·CPU self-time은 아니며 이전 실행과 focus/표본 빈도/캐시 읽기 조건이 달라 성능 A/B로 사용하지 않는다.

skeleton, transform array, matrix array, type-data pointer equality는 각각772/772 true다. 두 경로의 포즈 값도772/772 같다. 이번 대상·샘플링 시점에는 같은 저장소를 보고 있었다. 별도 두 pose 경로의 독립 검증으로 쓰지 않는다.

query와 interpolation skeleton의 dirty flag는772표본 모두1이었다.6개 본의 회전은 각724/771 인접쌍에서, 기존 MS cache translation은 각735/771쌍에서 변했다. 이 count 차이로 지연이나 버그를 계산하지 않는다. flag1은 포즈 갱신 후 캐시 재계산 전의 정상 상태일 수 있다.

서버 DONE·조기disconnect0·BattlEye kick0·crash0, client 종료 marker 확보·강제 종료 없음, 기존3개 PBO 서명 검증 성공이다. 알려진 기본 진단 플러그인 경고8줄은 보존했다. foreground는 요청했고 계측 중 모델 표시를 확인했으나 continuous appActive나 영상 cadence는 측정하지 않았다. 새8/64 회귀·frame-time p50/p95/p99·GPU/표시/할당/GC는 미측정이다.20ms p95는 목표다.

## 4. 원인 분석

확인된 사실: 이번 query/interpolated 일치는 alias 때문이다. 단, 현재 선택한 진단 개체·관찰 단계의 결과이며 모든 일반 빌드/개체로 확대하지 않는다. alias는 원래 설계일 수 있고 결함이라는 뜻은 아니다.

확인된 사실: 실제 MS API는 slot618이며 cache builder를 거친 skeleton+38의48-byte matrix translation을 반환한다. builder는 skeleton+A8이0이면 반환하고, 필요하면 skeleton+30의32-byte 변환 및 계층 정보를 바탕으로 행렬을 만들고 A8을0으로 내린다.873개 직접 도달 명령과 명시 창을 보존했으며 무한한 전역 알고리즘 확보 주장은 아니다.

따라서 compressed 성분과 MS cache를 직접 같은 좌표로 비교한 오차 계산은 부적합하다. 첫 Head의 계층 입력 y약0.07575와 기존 MS cache y약1.43812는 같은 의미의 좌표가 아니다. 기존 cache는 dirty1이므로 그 순간 최신 모델 포즈라는 보장도 없다. 관찰자가 강제 갱신하지 않았다는 점을 유지한다.

미확인: 자연적인 cache rebuild가 실제로 언제 실행되는지, 그 뒤 렌더·표시에 어떤 포즈가 쓰이는지, 일반 mode0의 시간 손실과 진단 mode2의 표시가 얼마나 다른지는 아직 연결되지 않았다. 이번 still로 움직임의 부드러움을 판정하지 않는다.

## 5. 검증 수준

실제 MS API binding·cache 경로 정정과 live pointer identity 확인은 진단 성공이다.15초1회이며 반복 안정성·전체 NPC 회귀가 아니다. 자연 캐시 갱신→렌더→표시 연결, 일반 exe 해결, 시각·성능 및 최종 수용은 미완료다.

## 6. 변경 사항

격리 observer와 producer·fixture·분석만 추가했다. 게임 코드·PBO·ANM·9350 후보·운영 서버는 변경하지 않았다. 기존3개 PBO 해시·서명 검증은 성공했고 새 PBO가 없어 신규 서명 대상은 없다. hook/injected 코드의 오버헤드가 있어 성능 비교에서 제외한다. 게임 수치 보정·cache 강제 호출·서비스/드라이버/보안 변경·새 설치·영상·대용량dump는 없다.

과거 원시 보고서/분석 결과는 덮어쓰지 않고 현재 해석에서 진단 slot610 MS 명칭을 철회했다. 이번 canonical 진행·공개 최신 요약에는 이 정정을 명시한다. 일반 exe의 MS slot610 자체를 잘못됐다고 하는 것은 아니다.

## 7. 원시 증거

분석 ID animation-diag-pose-identity-20261101-1740. bone-api-bindings-diag, space-paths의 선택 함수, ms-matrix-path, ms-cache-builder-expanded, runtime native-observer/heartbeat/launch/profile, foreground-ready/ui-observation/during-trace-window를 보존한다. 화면 SHA 6a3dc8efba72d8007c1db79ec1f394de22071e3a2586aa664d87c33d9f0a56e6이다. 정적 분석은 기존 bounded Capstone, runtime은 Python Frida이며 REA provider 전체 decompilation/runtime Evidence가 아니다.

공개본은 실제 binding·선택 함수·포인터 동일성 boolean·처음/마지막 표본과 한계를 제공한다. 포인터 절대 주소·PID·CLI·사용자 식별자·로컬 경로·전체 raw 로그/이미지·게임 자산·키는 제외한다. 입력/파일 해시·JSON·종료·안정성 확인 뒤 기존 체크포인트 체계에 저장했다.

## 8. 잔여 문제

다음은 해당 shared skeleton에 자연적으로 호출되는 C10FF0의 entry/exit와 return site를 관찰해 dirty flag·matrix 갱신 시점을 잇는 것이다. 강제로 API/cache 갱신을 호출하지 않는다. 기존 capture 인프라로 실제 표시와 동기화할 수 있는 재현 조건을 함께 준비한다.

단순 frame skipping·보간·시간 배율 변경은 보류한다. 진단/일반 빌드,1파트/전체 NPC,시각/기능/성능의 구분을 유지하며 기존 mode0 손실·원본 변환·64개체 비용 연구는 그대로 보존한다.

선정 공개 본문. 로컬 보고서 SHA-256: `fbccfb87c1dafc324b66d77ceb46a9c75a6a2a602c51c7d2239185ac24dc45fd`. 원시 전체가 아니다.
