# 자연 캐시 갱신과 실제 창 녹화의 시간 연결·한계

작성: 2026-11-01T18:09:09.544231+09:00

## 1. 작업 목표

이전 진단에서 확인한 공유 스켈레톤의 자연 캐시 갱신 시점을 관찰하고, 실제 게임 창 녹화와 QPC 시간축을 연결한다. 캐시 강제 갱신·시간 배율·새 보간은 적용하지 않는다.

## 2. 실제 수행

기존 OBS portable의 게임 창 전용·무음 60fps 녹화에 16비트 출력 프레임 표식과 QPC sidecar를 추가했다. Lua 경로의 Unicode 처리 실패와 scene 초기화 순서 실패는 각각 보존했다. 2초 preflight에서 123/123 영상 프레임 표식 해독에 성공했으나 게임 없는 배관 검사로만 취급한다.

첫 본시험은 launcher JSON 쓰기와 읽기가 겹쳐 기록기가 시작되지 않았다. 네이티브 hook도 연결되지 않았으며 게임 자체는 예정대로 종료됐다. 이를 NPC 실패로 분류하지 않는다. PS5 원자적 JSON 교체 helper와 읽기 재시도를 추가했다. 첫 null backup 인자 실패도 보존하고, 수정 후 실제 PS5 100회 쓰기·982회 읽기·JSON 오류0을 확인했다. v2는 sequence99 도달만 확인했고 정식 검사 영수증이 없어 성공 횟수로 쓰지 않았다.

두 번째 본시험은 기존 A500 진단 client/server, 1파트·고정run·AI/root 이동OFF 조건에서 수행했다. selected skeleton65의 scheduler·buffer·consumer·interpolation 및 자연 cache builder entry/exit를 hook 관찰했다. 24초 OBS 녹화와 동시 수집 후 정상 종료했다. 게임 PBO·ANM·AI 코드는 바꾸지 않았다. 호출 return site 두 곳의 PE unwind 범위를 기존 Capstone으로 제한 역어셈블했다.

## 3. 측정 결과

본시험2회 중 녹화+native 결합 성공1회다. 성공 실행의 표본 구간은15.5226175초·777표본이다. scheduler/buffer/consumer/interpolation 각777회, observer오류0이다. scheduler-exit callback 간격 p50/p95/p99=19.9226/22.83305/23.838725ms이며 실제 게임 frame-time이 아니다.

cache 함수1,553회: dirty진입1,088·clean진입465회. dirty 사건1,088/1,088에서 exit dirty0 및 Head MS translation 변화가 확인됐다. return RVA C07264에서777회, C127B8에서311회다. 각 직전 call이 C10FF0임을 확인했지만 상위 의미·renderer 소유권은 미확정이다. cache hook 경과시간 p50/p95/p99=17.1/29.13/36.326µs는 계측을 포함하는 wall 범위이며 self-time/최적화 근거로 쓰지 않는다.

영상24초·1,443프레임 모두 QPC sidecar 표식에 연결됐다. 표식 중복6건은 게임 프레임 드롭 증거가 아니다. 자연 cache·영상 겹침 구간931프레임의 고정100x100 ROI에서 직전과 완전 동일한156쌍을 관찰했다. ROI에는 머리·상체·배경·커서가 포함되므로 본 포즈의 갱신 빈도나 시각적 끊김 판정으로 쓰지 않는다. 비동기 game texture 나이·물리 scanout은 측정하지 못했다.

서버DONE·조기disconnect0, client 정상 종료 marker·강제종료없음, 기존3개 PBO 서명 검증 성공이다. OBS 정상 녹화종료·소유 recorder 종료·기존 profile/scene byte 복원 확인을 마쳤다. 기본 진단 plugin 경고는 보존했다. 새8/64 기능 회귀·실제 frame-time p50/p95/p99·GPU/할당/GC는 미측정이고20ms p95는 목표다.

## 4. 원인 분석

확인된 사실: 선택 대상에서 자연 cache rebuild가 실행되고 dirty1→0 및 행렬 변화가 발생한다. 이전 scheduler-exit의 dirty1을 캐시가 영구 정지한 결함으로 해석할 수 없다. 다만 이 결과는 전체 일반 실행의 stutter 원인을 반증하는 것은 아니다.

확인된 사실: OBS 출력 scene tick과 QPC의 연결은 가능하다. 그러나 이 표식은 비동기 Window Capture가 가져온 게임 texture의 생성시각을 표시하지 않는다. ROI 반복을 native 정지나 렌더 지연으로 곧바로 연결할 수 없다.

미확인: 두 cache 호출자의 상위 의미, render submission에 쓰인 포즈의 식별자, captured texture의 나이, 화면에서 추적한 동일 본 특징과 모델 좌표 투영의 관계다. 강제 cache 갱신·새 clock 주입·프레임 생략 등 해결책을 이번 근거로 정당화하지 않는다.

## 5. 검증 수준

자연 캐시 갱신 관찰과 OBS 출력 표식/QPC 연결은 진단 성공이다. 결합 성공1회이며 반복 검증 완료·전체 NPC 회귀·시각 품질·성능·최종 수용은 아니다. ROI 영상 내용은 직접 측정했지만 개별 애니메이션의 부드러움 개선은 확인하지 못했다.

## 6. 변경 사항

기존 격리 fixture의 상태 파일 출력을 원자적 교체로 수정하고 retry 읽기를 추가했다. 별도 cache observer·OBS Lua·해독/분석 스크립트를 추가했다. 녹화 중 임시 portable 설정은 원래 bytes로 복원했다. 기존 game/PBO·9350 후보·운영 서버·Steam 배포본은 변경하지 않았다. 신규 PBO 서명 대상은 없으며 고정3개 서명 검증은 성공이다.

## 7. 원시 증거

실행 묶음 animation-diag-cache-visible-20261101-1750. runtime는 오케스트레이션 실패, runtime-2는 결합 성공이다. launch/profile/native-observer/heartbeat/markers/decoded-markers/roi-frame-observations/recorder-result/분석 및 return-sites를 보존한다. 본영상 SHA256=9354604a9746687934ce25f431784f70a9b3c240d0da7c7f2071dbd6257cc592, 크기534735132bytes다. 새 손실 없는 영상이 약510MiB이므로 추가 full-frame 파생 영상은 만들지 않고 작은 ROI 관측값만 생성했다. 이번 영상은 원시 증거로 보존하며 자동 삭제하지 않는다.

정적 경로는 bounded Capstone, runtime hook은 기존 Python Frida이며 REA의 전체 runtime Evidence가 아니다. 공개본에는 집계·선정 호출 명령·해시·한계만 포함하고 로컬 경로·PID·CLI·절대 런타임 포인터·원시 영상·게임 자산·키를 제외한다. 저장 전 해시와 JSON 및 종료 상태를 검증했다.

## 8. 잔여 문제

다음은 확보 영상에서 머리의 동일 시각 특징을 보정하고 카메라 투영과 MS 좌표를 연결하는 분석이다. 동시에 두 cache caller의 상위 역할과 WGC texture 나이의 측정 가능 범위를 좁힌다. 출력표식만으로 실제 GPU/scanout까지 연결됐다고 보고하지 않는다. 새 녹화보다 기존 영상의 국소 분석을 먼저 활용한다.

일반 mode0 누적기 손실·진단 mode2·전체8파트·64개체 비용은 별도 실험으로 유지한다. 이번 hook/녹화 부하는 성능 비교에서 제외하고20ms p95나 시각 개선을 주장하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `78cf30bb7c92a1fb39073d6b4c0120500fe00d4bbec7e780e8f354df54f77341`. 원시 전체가 아니다.
